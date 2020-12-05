class FASTPIX3D_API Input
{
private:
	static bool Quit;
	static int32 MouseX;
	static int32 MouseY;
	static int32 MouseZ;
	static int32 MouseXSpeed;
	static int32 MouseYSpeed;
	static int32 MouseZSpeed;
	static bool *MouseDown;
	static bool *KeyDown;

	static void Initialize();
	static void Destroy();
public:
	static void Update();

	static bool getQuit();
	static int32 getMouseX();
	static int32 getMouseY();
	static int32 getMouseZ();
	static int32 getMouseXSpeed();
	static int32 getMouseYSpeed();
	static int32 getMouseZSpeed();
	static bool getMouseDown(MouseButton mouseButton);
	static bool getKeyDown(int32 key);

	static void setMousePosition(int32 x, int32 y, bool updateMouseSpeed);

	friend class Device;
};