class FASTPIX3D_API FPSCounter
{
private:
	int32 Interval, FPS, FPSCount, FrameTime, MedianFrameTime;
	clock_t LastMeasuredTime, LastFrame;
public:
	FPSCounter();
	FPSCounter(int32 interval);

	void Frame();

	int32 getInterval();
	int32 getFPS();
	int32 getFrameTime();
	int32 getMedianFrameTime();

	void setInterval(int32 interval);
};