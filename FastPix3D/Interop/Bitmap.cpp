#include "Bitmap.h"
#include "../Math/vuint8.h"
#include <wincodec.h>

INIT_ONCE Bitmap::InitOnce = INIT_ONCE_STATIC_INIT;
IWICImagingFactory *Bitmap::ImagingFactory;

Bitmap::Bitmap(int32 width, int32 height) :
	_Width(width),
	_Height(height),
	_Pixels((Color*)_aligned_malloc(width * height * sizeof(Color), 32))
{
}
Bitmap::Bitmap(const Bitmap &other) :
	_Width(other._Width),
	_Height(other._Height),
	_Pixels((Color*)_aligned_malloc(other._Width * other._Height * sizeof(Color), 32))
{
	memcpy(_Pixels, other._Pixels, other._Width * other._Height * sizeof(Color));
}
Bitmap::~Bitmap()
{
	_aligned_free(_Pixels);
}

Bitmap* Bitmap::FromFile(const char *path)
{
	if (!path) throw std::invalid_argument("path cannot be null.");

	EnsureInitialized();

	Bitmap *bitmap = nullptr;

	IStream *stream;
	if (SUCCEEDED(SHCreateStreamOnFileA(path, STGM_READ | STGM_SHARE_DENY_WRITE, &stream)))
	{
		bitmap = FromStream(stream);
		stream->Release();
	}

	return bitmap;
}
Bitmap* Bitmap::FromMemory(const void *buffer, int32 size)
{
	if (!buffer) throw std::invalid_argument("buffer cannot be null.");
	if (size <= 0) throw std::invalid_argument("size must be greater than zero.");

	EnsureInitialized();

	Bitmap *bitmap = nullptr;

	IWICStream *stream;
	if (SUCCEEDED(ImagingFactory->CreateStream(&stream)))
	{
		if (SUCCEEDED(stream->InitializeFromMemory((BYTE*)buffer, (DWORD)size)))
		{
			bitmap = FromStream(stream);
		}

		stream->Release();
	}

	return bitmap;
}
Bitmap* Bitmap::FromStream(IStream *stream)
{
	Bitmap *bitmap = nullptr;

	IWICBitmapDecoder *decoder;
	if (SUCCEEDED(ImagingFactory->CreateDecoderFromStream(stream, nullptr, WICDecodeMetadataCacheOnDemand, &decoder)))
	{
		IWICBitmapFrameDecode *frame = nullptr;
		if (SUCCEEDED(decoder->GetFrame(0, &frame)))
		{
			UINT width;
			UINT height;
			if (SUCCEEDED(frame->GetSize(&width, &height)))
			{
				IWICFormatConverter *converter;
				if (SUCCEEDED(ImagingFactory->CreateFormatConverter(&converter)))
				{
					if (SUCCEEDED(converter->Initialize(frame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0.0, WICBitmapPaletteTypeCustom)))
					{
						bitmap = new Bitmap((int32)width, (int32)height);

						if (SUCCEEDED(converter->CopyPixels(nullptr, width * sizeof(Color), width * height * sizeof(Color), (LPBYTE)bitmap->Pixels)))
						{
							// Remove alpha channel to avoid spilling of alpha values into the RGB channels.
							int32 *ptr = (int32*)bitmap->Pixels;
							int32 count = bitmap->_Width * bitmap->_Height;
							vuint8 mask = vuint8(0xffffff);

							while (count >= 8)
							{
								vuint8::Write((uint32*)ptr, vuint8((vuint8*)ptr) & mask);
								ptr += 8;
								count -= 8;
							}

							while (count--)
							{
								*ptr++ &= 0xffffff;
							}
						}
						else
						{
							delete bitmap;
							bitmap = nullptr;
						}
					}

					converter->Release();
				}
			}

			frame->Release();
		}

		decoder->Release();
	}

	return bitmap;
}

void Bitmap::EnsureInitialized()
{
	if (!InitOnceExecuteOnce(&InitOnce, Initialize, nullptr, nullptr))
	{
		throw std::runtime_error("Failed to initialize COM.");
	}
}
BOOL CALLBACK Bitmap::Initialize(PINIT_ONCE initOnce, PVOID parameter, PVOID *context)
{
	HRESULT result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	bool uninitializeCom = SUCCEEDED(result);

	if (FAILED(result) && result != RPC_E_CHANGED_MODE)
	{
		return FALSE;
	}

	if (FAILED(CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&ImagingFactory))))
	{
		if (uninitializeCom)
		{
			CoUninitialize();
		}

		return FALSE;
	}

	return TRUE;
}

Bitmap& Bitmap::operator =(const Bitmap &other)
{
	if (this != &other)
	{
		_aligned_free(_Pixels);

		_Width = other._Width;
		_Height = other._Height;
		_Pixels = (Color*)_aligned_malloc(other._Width * other._Height * sizeof(Color), 32);
		memcpy(_Pixels, other._Pixels, other._Width * other._Height * sizeof(Color));
	}

	return *this;
}