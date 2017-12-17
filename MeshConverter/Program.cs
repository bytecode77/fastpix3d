using System;
using System.IO;
using System.Runtime.InteropServices;

namespace MeshConverter
{
	public static class Program
	{
		public static void Main(string[] args)
		{
			string fileName = "";

			foreach (string arg in args)
			{
				if (File.Exists(arg)) fileName = arg;
			}

			if (args.Length == 0)
			{
				Console.WriteLine("Please submit a *.b3d, *.3ds or *.x file via commandline.");
			}
			else if (!File.Exists(fileName))
			{
				Console.WriteLine("This file does not exist:\r\n" + fileName);
			}
			else
			{
				string file = Path.Combine(Path.GetDirectoryName(fileName), Path.GetFileNameWithoutExtension(fileName)) + ".fp3d";
				FileStream fileStream = File.Create(file);
				BinaryWriter bin = new BinaryWriter(fileStream);
				WriteString(bin, "FastPix3D_Mesh_1.0.2");

				try
				{
					bb.BeginBlitz3D();
					bb.Graphics3D(320, 240, 32, 2);
					int mesh = bb.LoadMesh(fileName);

					int surfaceCount = bb.CountSurfaces(mesh);
					bin.Write((ushort)surfaceCount);
					for (int i = 1; i <= surfaceCount; i++)
					{
						int surface = bb.GetSurface(mesh, i);

						WriteString(bin, Path.GetFileName(bbTextureName(bb.GetBrushTexture(bb.GetSurfaceBrush(surface)))));

						int vertexCount = bb.CountVertices(surface), triangleCount = bb.CountTriangles(surface);
						bin.Write((ushort)vertexCount);
						bin.Write((ushort)triangleCount);

						for (int a = 0; a < vertexCount; a++)
						{
							bin.Write(bb.VertexX(surface, a));
							bin.Write(bb.VertexY(surface, a));
							bin.Write(bb.VertexZ(surface, a));
							bin.Write(bb.VertexNX(surface, a));
							bin.Write(bb.VertexNY(surface, a));
							bin.Write(bb.VertexNZ(surface, a));
							bin.Write(bb.VertexU(surface, a));
							bin.Write(bb.VertexV(surface, a));
							bin.Write((byte)bb.VertexRed(surface, a));
							bin.Write((byte)bb.VertexGreen(surface, a));
							bin.Write((byte)bb.VertexBlue(surface, a));
						}
						for (int a = 0; a < triangleCount; a++)
						{
							bin.Write((ushort)bb.TriangleVertex(surface, a, 0));
							bin.Write((ushort)bb.TriangleVertex(surface, a, 1));
							bin.Write((ushort)bb.TriangleVertex(surface, a, 2));
						}
					}

					bin.Close();
					fileStream.Close();
				}
				catch (Exception ex)
				{
					Console.WriteLine(ex.ToString() + "\r\n\r\n");
					Console.WriteLine(ex.Message);
					bin.Close();
					fileStream.Close();
					File.Delete(file);
				}
			}
		}

		private static void WriteString(BinaryWriter bin, string text)
		{
			bin.Write(text.ToCharArray());
			bin.Write('\0');
		}

		[DllImport("Blitz3D.dll")]
		[return: MarshalAs(UnmanagedType.AnsiBStr)]
		private static extern string bbTextureName(int texture);
	}
}