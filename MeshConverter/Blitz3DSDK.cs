using System;
using System.Runtime.InteropServices;

namespace MeshConverter
{
	static public class bb
	{

		[DllImport("Blitz3D.dll")]
		public static extern int bbBeginBlitz3D();
		/// <summary>
		/// <para>
		/// Initializes the Blitz3D runtime environment and returns 0
		/// if there is an error.
		/// </para>
		/// </summary>
		public static int BeginBlitz3D() { return bbBeginBlitz3D(); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbBeginBlitz3DEx(int hwnd, int flags);
		/// <summary>
		/// <para>
		/// Initializes the Blitz3D runtime environment and returns 0
		/// if there is an error.
		/// </para>
		/// </summary>
		public static int BeginBlitz3DEx(int hwnd, int flags) { return bbBeginBlitz3DEx(hwnd, flags); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbEndBlitz3D();
		/// <summary>
		/// <para>
		/// Closes down the Blitz3D runtime environment and releases
		/// all resources.
		/// </para>
		/// </summary>
		public static int EndBlitz3D() { return bbEndBlitz3D(); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbValidateGraphics();
		/// <summary>
		/// <para>
		/// Initializes the Blitz3D runtime environment and returns 0
		/// if there is an error.
		/// </para>
		/// </summary>
		public static int ValidateGraphics() { return bbValidateGraphics(); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbSetBlitz3DDebugMode(int debugmode);
		/// <summary>
		/// <para>
		/// Sets the debugging level for the Blitz3DSDK. A value of 0
		/// can be used for release builds that have been fully debugged
		/// and do not require runtime checks on all parameters
		/// passed to Blitz3DSDK functions.
		/// </para>
		/// </summary>
		public static int SetBlitz3DDebugMode(int debugmode) { return bbSetBlitz3DDebugMode(debugmode); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbSetBlitz3DDebugCallback(int callback);
		/// <summary>
		/// <para>
		/// Installs an error handler for use by the Blitz3DSDK.
		/// </para>
		/// <para>
		/// The prototype for the callback is 
		/// </para>
		/// <para>
		/// </para>
		/// <para>
		/// The Blitz3D runtime is closed before the error handler is invoked by the Blitz3DSDK.
		/// </para>
		/// <para>
		/// The BlitzMax Blitz3DSDK module installs a default error handler which throws a
		/// BlitzMax RuntimError containing the error message.
		/// </para>
		/// </summary>
		public static int SetBlitz3DDebugCallback(int callback) { return bbSetBlitz3DDebugCallback(callback); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbSetBlitz3DEventCallback(int callback);
		/// <summary>
		/// <para>
		/// Installs an event handler for use by the Blitz3DSDK. 
		/// </para>
		/// <para>
		/// <c>SetBlitz3DEventCallback</c> must be called before <c>BeginBlitz3D.</c>
		/// </para>
		/// <para>
		/// The prototype for the callback is:
		/// </para>
		/// <para>
		/// </para>
		/// <para>
		/// The following BlitzMax code demonstrates the use of an eventhandler which
		/// enables standard BlitzMax input commands to be used with the Blitz3DSDK.
		/// </para>
		/// <para>
		/// </para>
		/// </summary>
		public static int SetBlitz3DEventCallback(int callback) { return bbSetBlitz3DEventCallback(callback); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbSetBlitz3DHWND(int hwndparent);
		/// <summary>
		/// <para>
		/// Specifies a parent window for the Blitz3DSDK to use with
		/// the windowed modes of the <c>Graphics3D</c> command.
		/// </para>
		/// <para>
		/// <c>SetBlitz3DHWND</c> must be called before <c>BeginBlitz3D.</c>
		/// </para>
		/// <para>
		/// Call <c>Graphics3D</c> using the <c>GFX_WINDOWED</c> mode and
		/// the maximum size of the parent window and use the
		/// <c>SetViewport</c> command to configure the 3D display
		/// to the parent window's current size.
		/// </para>
		/// </summary>
		public static int SetBlitz3DHWND(int hwndparent) { return bbSetBlitz3DHWND(hwndparent); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbSetBlitz3DTitle(String windowtitle, String quitmessage);
		/// <summary>
		/// <para>
		/// Specifies a title for any windows created by the Blitz3DSDK
		/// and optionally a message that is displayed when the user closes
		/// a window created by the Blitz3DSDK.
		/// </para>
		/// </summary>
		public static int SetBlitz3DTitle(String windowtitle, String quitmessage) { return bbSetBlitz3DTitle(windowtitle, quitmessage); }
		public static int SetBlitz3DTitle(String windowtitle) { return bbSetBlitz3DTitle(windowtitle, ""); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbGraphics(int width, int height, int depth, int mode);
		/// <summary>
		/// <para>
		/// The <c>Graphics</c> command resizes the graphics display to the 
		/// specified size in pixels and with the specified display 
		/// properties including the color depth and fullscreen options.
		/// </para>
		/// <para>
		/// When a Blitz program begins a default 400x300 pixel 
		/// graphics window is created.
		/// </para>
		/// <para>
		/// The depth parameter is optional, the default value of 0
		/// specifies that Blitz3D select the most appropriate color
		/// depth.
		/// </para>
		/// <para>
		/// The mode parameter may be any of the following values:
		/// </para>
		/// <list type="table">
		/// <listheader><term>Mode</term><term>Value</term><term>Description</term></listheader>
		/// <item><description>GFX_DEFAULT</description></item><item><description>0</description></item><item><description>FixedWindowed in Debug mode and FullScreen in Release</description></item>
		/// <item><description>GFX_FULLSCREEN</description></item><item><description>1</description></item><item><description>Own entire screen for optimal performance</description></item>
		/// <item><description>GFX_WINDOWED</description></item><item><description>2</description></item><item><description>A fixed size window placed on the desktop</description></item>
		/// <item><description>GFX_WINDOWEDSCALED</description></item><item><description>3</description></item><item><description>Graphics scaled according to current size of Window</description></item>
		/// </list>
		/// <para>
		/// Before using <c>Graphics</c> to configure a fullscreen display,
		/// the specified resolution should be verified as available
		/// on the current graphics driver.
		/// </para>
		/// <para>
		/// The <c>GfxModeExists</c> function will return <c>False</c> if the specified
		/// resolution is not available. Calling <c>Graphics</c> with an unsupported
		/// resolution will cause the program to fail with an &quot;Unable to Set 
		/// Graphics Mode&quot; error message.
		/// </para>
		/// <para>
		/// The <c>Graphics</c> command causes all images to be destroyed meaning
		/// all images should be (re)loaded after any use of the <c>Graphics</c> command.
		/// </para>
		/// <para>
		/// See the Blitz3D command <c>Graphics3D</c> for configuring the display
		/// for use with 3D graphics.
		/// </para>
		/// </summary>
		/// <param name="width">width of display in pixels</param>
		/// <param name="height">height of display in pixels</param>
		/// <param name="depth">color depth in bits, 0 = default</param>
		/// <param name="mode">video mode flags, 0 = default</param>
		public static int Graphics(int width, int height, int depth, int mode) { return bbGraphics(width, height, depth, mode); }
		public static int Graphics(int width, int height, int depth) { return bbGraphics(width, height, depth, 0); }
		public static int Graphics(int width, int height) { return bbGraphics(width, height, 0, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbFlip(int synch);
		/// <summary>
		/// <para>
		/// The <c>Flip</c> command switches the FrontBuffer() and BackBuffer() 
		/// of the current <c>Graphics</c> display.
		/// </para>
		/// <para>
		/// See the <c>BackBuffer</c> command for a description on setting a 
		/// standard <c>Graphics</c> display up for double buffered drawing.
		/// </para>
		/// <para>
		/// The ability to draw graphics to a hidden buffer and then
		/// transfer the completed drawing to the display is called 
		/// double buffering.
		/// </para>
		/// <para>
		/// The <c>Flip</c> command is used at the end of each drawing cycle 
		/// to display the results onto the display in a flicker free 
		/// manner.
		/// </para>
		/// <para>
		/// The optional <c>synch</c> value may be set to False to override
		/// the default True setting. Unsynchronized flipping should
		/// only ever be used monitoring rendering performance as
		/// it results in an ugly screen tearing.
		/// </para>
		/// </summary>
		/// <param name="synch"><c>True</c> (default) to synchronize flip to display</param>
		public static int Flip(int synch) { return bbFlip(synch); }
		public static int Flip() { return bbFlip(1); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbBackBuffer();
		/// <summary>
		/// <para>
		/// The <c>BackBuffer</c> function returns a buffer that corresponds
		/// to the hidden area that will be flipped to the display when
		/// the <c>Flip</c> command is called.
		/// </para>
		/// <para>
		/// The <c>BackBuffer</c> is the current drawing buffer after a call
		/// to <c>Graphics3D.</c>
		/// </para>
		/// <para>
		/// Unlike <c>Graphics3D</c> the <c>Graphics</c> command does not make the
		/// <c>BackBuffer</c> the default drawing surface so a <c>SetBuffer</c> <c>BackBuffer</c>
		/// command sequence is required after the <c>Graphics</c> command
		/// in order for the display to be configured for double buffered drawing.
		/// </para>
		/// </summary>
		public static int BackBuffer() { return bbBackBuffer(); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbFrontBuffer();
		/// <summary>
		/// <para>
		/// The <c>FrontBuffer</c> function returns a buffer that corresponds
		/// to that viewable on the display.
		/// </para>
		/// <para>
		/// Drawing to the FrontBuffer() can be used to display an image
		/// that is progressively rendered. That is each main loop the
		/// program does not include a <c>Cls</c> or <c>Flip</c> but continually draws
		/// to the FrontBuffer allowing the user to view the image as it
		/// is created over the period of minutes or hours.
		/// </para>
		/// </summary>
		public static int FrontBuffer() { return bbFrontBuffer(); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbGraphicsWidth();
		/// <summary>
		/// <para>
		/// The <c>GraphicsWidth</c> command returns the current width of the
		/// display in pixels.
		/// </para>
		/// </summary>
		public static int GraphicsWidth() { return bbGraphicsWidth(); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbGraphicsHeight();
		/// <summary>
		/// <para>
		/// The <c>GraphicsHeight</c> command returns the current height of the
		/// display in pixels.
		/// </para>
		/// </summary>
		public static int GraphicsHeight() { return bbGraphicsHeight(); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbGraphicsDepth();
		/// <summary>
		/// <para>
		/// The <c>GraphicsDepth</c> command returns the current color depth of the
		/// display.
		/// </para>
		/// </summary>
		public static int GraphicsDepth() { return bbGraphicsDepth(); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbEndGraphics();
		/// <summary>
		/// <para>
		/// Returns the Graphics mode to the original 400x300 fixed window.
		/// </para>
		/// <para>
		/// The <c>EndGraphics</c> command causes all images to be destroyed.
		/// </para>
		/// </summary>
		public static int EndGraphics() { return bbEndGraphics(); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbVWait(int frames);
		/// <summary>
		/// <para>
		/// <c>VWait</c> will cause the program to halt execution until the
		/// video display has completed its refresh and reached it's
		/// Vertical Blank cycle (the time during which the video
		/// beam returns to the top of the display to begin its next
		/// refresh).
		/// </para>
		/// <para>
		/// The <c>VWait</c> command provides an alternative method to using
		/// the synchronized version of the <c>Flip</c> command (default)
		/// which is useful on vintage computer hardware that does
		/// not provide a properly synchonized Flip response.
		/// </para>
		/// <para>
		/// Synching a game's display using the VWait command will also
		/// cause the program to exhibit excess CPU usage and should
		/// be made optional if utilized at all.
		/// </para>
		/// </summary>
		/// <param name="frames">optional number of frames to wait. Default is 1</param>
		public static int VWait(int frames) { return bbVWait(frames); }
		public static int VWait() { return bbVWait(1); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbScanLine();
		/// <summary>
		/// <para>
		/// The <c>ScanLine</c> function returns the actual scanline being
		/// refreshed by the video hardware or 0 if in vertical blank
		/// or unsupported by the hardware.
		/// </para>
		/// </summary>
		public static int ScanLine() { return bbScanLine(); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbTotalVidMem();
		/// <summary>
		/// <para>
		/// Returns the total amount of graphics memory present on the current graphics
		/// device.
		/// </para>
		/// <para>
		/// Use the <c>AvailVidMem</c> command to find the available amount of video memory
		/// and the difference to calculate the amount of video memory currently in
		/// use.
		/// </para>
		/// </summary>
		public static int TotalVidMem() { return bbTotalVidMem(); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbAvailVidMem();
		/// <summary>
		/// <para>
		/// Returns the available amount of graphics memory on the current graphics
		/// device.
		/// </para>
		/// </summary>
		public static int AvailVidMem() { return bbAvailVidMem(); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbSetGamma(int red, int green, int blue, int dest_red, int dest_green, int dest_blue);
		/// <summary>
		/// <para>
		/// SetGamma allows you to modify the gamma tables.
		/// </para>
		/// <para>
		/// Gamma can ONLY be used in fullscreen mode.
		/// </para>
		/// <para>
		/// Gamma is performed on a per channel basis, with each red, green and blue
		/// channel using a translation table of 256 entries to modify the resultant
		/// color output. The <c>SetGamma</c> command allows you to modify the specified
		/// entry with the specified value for each of the 3 channels.
		/// </para>
		/// <para>
		/// Suitable translation tables can be configured to influence
		/// any or all of the 3 primary color components allowing the
		/// displayed channel (red, green or blue) to be amplified, 
		/// muted or even inverted.
		/// </para>
		/// <para>
		/// After performing one or more <c>SetGamma</c> commands, call <c>UpdateGamma</c> in 
		/// order for the changes to become effective.
		/// </para>
		/// </summary>
		/// <param name="red">red input value</param>
		/// <param name="green">green input value</param>
		/// <param name="blue">blue input value</param>
		/// <param name="dest_red">red output value</param>
		/// <param name="dest_green">green output value</param>
		/// <param name="dest_blue">blue output value</param>
		public static int SetGamma(int red, int green, int blue, int dest_red, int dest_green, int dest_blue) { return bbSetGamma(red, green, blue, dest_red, dest_green, dest_blue); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbUpdateGamma(int calibrate);
		/// <summary>
		/// <para>
		/// UpdateGamma should be used after a series of <c>SetGamma</c> commands in order to 
		/// effect actual changes.
		/// </para>
		/// </summary>
		/// <param name="calibrate">True if the gamma table should be calibrated to the display</param>
		public static int UpdateGamma(int calibrate) { return bbUpdateGamma(calibrate); }
		public static int UpdateGamma() { return bbUpdateGamma(0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbGammaRed(int value);
		/// <summary>
		/// <para>
		/// Returns the adjusted output value of the red channel given the 
		/// specified input <c>value</c> by referencing the current gamma correction
		/// tables.
		/// </para>
		/// <para>
		/// See <c>SetGamma</c> for more information
		/// </para>
		/// </summary>
		/// <param name="value">an integer index into the red gamma table</param>
		public static int GammaRed(int value) { return bbGammaRed(value); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbGammaGreen(int value);
		/// <summary>
		/// <para>
		/// Returns the adjusted output value of the green channel given the 
		/// specified input <c>value</c> by referencing the current gamma correction
		/// tables.
		/// </para>
		/// <para>
		/// See <c>SetGamma</c> for more information
		/// </para>
		/// </summary>
		/// <param name="value">an integer index into the green gamma table</param>
		public static int GammaGreen(int value) { return bbGammaGreen(value); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbGammaBlue(int value);
		/// <summary>
		/// <para>
		/// Returns the adjusted output value of the blue channel given the 
		/// specified input <c>value</c> by referencing the current gamma correction
		/// tables.
		/// </para>
		/// <para>
		/// See <c>SetGamma</c> for more information
		/// </para>
		/// </summary>
		/// <param name="value">an integer index into the blue gamma table</param>
		public static int GammaBlue(int value) { return bbGammaBlue(value); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbPrint(String value);
		/// <summary>
		/// <para>
		/// The <c>Print</c> command writes a string version of <c>value</c> if specified to the 
		/// current graphics buffer at the current cursor position and moves the 
		/// cursor position to the next line.
		/// </para>
		/// <para>
		/// If the optional value parameter is omitted the <c>Print</c> command simply
		/// moves the cursor position down a line.
		/// </para>
		/// <para>
		/// As Blitz automatically converts any numeric or custom type to a string
		/// the value parameter can in fact be any value.
		/// </para>
		/// </summary>
		/// <param name="value">the text to be output (optional)</param>
		public static int Print(String value) { return bbPrint(value); }
		public static int Print() { return bbPrint(""); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbWrite(String value);
		/// <summary>
		/// <para>
		/// The <c>Write</c> command is similar to the <c>Print</c> command but the cursor
		/// is not moved to a new line at the completion of the command.
		/// </para>
		/// <para>
		/// Instead, the cursor is moved to the end of the output text.
		/// </para>
		/// </summary>
		/// <param name="value">the text to be output (optional)</param>
		public static int Write(String value) { return bbWrite(value); }

		[DllImport("Blitz3D.dll")]
		public static extern String bbInput(String prompt);
		/// <summary>
		/// <para>
		/// The <c>Input</c> command accepts and prints keyboard entry from the user
		/// until a Return key is received at which time the Input command returns
		/// a string result.
		/// </para>
		/// </summary>
		/// <param name="prompt">optional text to be printed before keyboard input proceeds</param>
		public static String Input(String prompt) { return bbInput(prompt); }
		public static String Input() { return bbInput(">"); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbLocate(int x, int y);
		/// <summary>
		/// <para>
		/// The <c>Locate</c> command positions the cursor position at the specified
		/// pixel position of the current graphics buffer.
		/// </para>
		/// </summary>
		/// <param name="x">horizontal position on the current graphics buffer in pixels</param>
		/// <param name="y">vertical position on the current graphics buffer in pixels</param>
		public static int Locate(int x, int y) { return bbLocate(x, y); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbCls();
		/// <summary>
		/// <para>
		/// The <c>Cls</c> command clears the current graphics buffer clean, using 
		/// an optional color specified with a previous call to <c>ClsColor.</c>
		/// </para>
		/// <para>
		/// <c>Cls</c> is not commonly called when using <c>Graphics3D</c> due to the
		/// behavior of <c>RenderWorld</c> which clears the <c>BackBuffer</c>
		/// using the various <c>CameraClsMode</c> settings instead.
		/// </para>
		/// </summary>
		public static int Cls() { return bbCls(); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbPlot(int x, int y);
		/// <summary>
		/// <para>
		/// <c>Plot</c> draws a single pixel at the coordinates specified 
		/// using the current drawing color.
		/// </para>
		/// </summary>
		/// <param name="x">horizontal pixel position</param>
		/// <param name="y">vertical pixel position</param>
		public static int Plot(int x, int y) { return bbPlot(x, y); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbLine(int x1, int y1, int x2, int y2);
		/// <summary>
		/// <para>
		/// The <c>Line</c> command draws a line, in the current drawing color, 
		/// from one pixel position to another.
		/// </para>
		/// </summary>
		/// <param name="x1">start pixel's horizontal position</param>
		/// <param name="y1">start pixel's vertical position</param>
		/// <param name="x2">end pixel's horizontal position</param>
		/// <param name="y2">end pixel's vertical position</param>
		public static int Line(int x1, int y1, int x2, int y2) { return bbLine(x1, y1, x2, y2); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbRect(int x, int y, int width, int height, int solid);
		/// <summary>
		/// <para>
		/// The <c>Rect</c> command draws a rectangle.
		/// </para>
		/// <para>
		/// It uses the current drawing color to draw a solid rectangle or
		/// outlined if a False setting is specified for the <c>solid</c> parameter.
		/// </para>
		/// </summary>
		/// <param name="x">horizontl pixel position</param>
		/// <param name="y">vertical pixel position</param>
		/// <param name="width">width in pixels</param>
		/// <param name="height">height in pixels</param>
		/// <param name="solid">False draws an outline only</param>
		public static int Rect(int x, int y, int width, int height, int solid) { return bbRect(x, y, width, height, solid); }
		public static int Rect(int x, int y, int width, int height) { return bbRect(x, y, width, height, 1); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbOval(int x, int y, int width, int height, int solid);
		/// <summary>
		/// <para>
		/// The <c>Oval</c> command can be used to draw circles and ovals in 
		/// solid or outline form.
		/// </para>
		/// <para>
		/// The shape of the <c>Oval</c> drawn is the largest that can fit inside
		/// the specified rectangle.
		/// </para>
		/// </summary>
		/// <param name="x">horizontl pixel position</param>
		/// <param name="y">vertical pixel position</param>
		/// <param name="width">width in pixels</param>
		/// <param name="height">height in pixels</param>
		/// <param name="solid">False draws an outline only</param>
		public static int Oval(int x, int y, int width, int height, int solid) { return bbOval(x, y, width, height, solid); }
		public static int Oval(int x, int y, int width, int height) { return bbOval(x, y, width, height, 1); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbColor(int red, int green, int blue);
		/// <summary>
		/// <para>
		/// This command sets the current drawing color allowing Lines,
		/// Rectangles, Ovals, Pixels and Text to be drawn in any color of the
		/// rainbow.
		/// </para>
		/// <para>
		/// The actual color is specified by 3 numbers representing
		/// the amount of red, green and blue mixed together.
		/// </para>
		/// <para>
		/// The following table demonstrates values of red, green and blue
		/// required to specify the named colors:
		/// </para>
		/// <list type="table">
		/// <listheader><term>Color</term><term>Red</term><term>Green</term><term>Blue</term></listheader>
		/// <item><description>Black</description></item><item><description>0</description></item><item><description>0</description></item><item><description>0</description></item>
		/// <item><description>Red</description></item><item><description>255</description></item><item><description>0</description></item><item><description>0</description></item>
		/// <item><description>Green</description></item><item><description>0</description></item><item><description>255</description></item><item><description>0</description></item>
		/// <item><description>Blue</description></item><item><description>0</description></item><item><description>0</description></item><item><description>255</description></item>
		/// <item><description>Yellow</description></item><item><description>255</description></item><item><description>255</description></item><item><description>0</description></item>
		/// <item><description>Turquoise</description></item><item><description>0</description></item><item><description>255</description></item><item><description>255</description></item>
		/// <item><description>Purple</description></item><item><description>255</description></item><item><description>0</description></item><item><description>255</description></item>
		/// <item><description>White</description></item><item><description>255</description></item><item><description>255</description></item><item><description>255</description></item>
		/// </list>
		/// </summary>
		/// <param name="red">amount of red (0..255)</param>
		/// <param name="green">amount of green (0..255)</param>
		/// <param name="blue">amount of blue (0..255)</param>
		public static int Color(int red, int green, int blue) { return bbColor(red, green, blue); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbClsColor(int red, int green, int blue);
		/// <summary>
		/// <para>
		/// The <c>ClsColor</c> command is used to change the Color used by the <c>Cls</c>
		/// command.
		/// </para>
		/// <para>
		/// See the <c>Color</c> command for combining values of red, green and blue
		/// in order to specify some commonly used colors.
		/// </para>
		/// </summary>
		/// <param name="red">amount of red (0..255)</param>
		/// <param name="green">amount of green (0..255)</param>
		/// <param name="blue">amount of blue (0..255)</param>
		public static int ClsColor(int red, int green, int blue) { return bbClsColor(red, green, blue); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbOrigin(int x, int y);
		/// <summary>
		/// <para>
		/// The <c>Origin</c> command sets a point of origin for all subsequent 
		/// drawing commands.
		/// </para>
		/// <para>
		/// The default <c>Origin</c> of a drawing buffer is the top left pixel.
		/// </para>
		/// <para>
		/// After calling <c>Origin,</c> all drawing commands will treat the pixel
		/// at location <c>x,<c>y</c></c> as coordinate 0,0
		/// </para>
		/// </summary>
		/// <param name="x">horizontal pixel position</param>
		/// <param name="y">vertical pixel position</param>
		public static int Origin(int x, int y) { return bbOrigin(x, y); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbViewport(int x, int y, int width, int height);
		/// <summary>
		/// <para>
		/// The <c>Viewport</c> command allows the cropping of subsequent drawing commands
		/// to a rectangular region of the current graphics buffer.
		/// </para>
		/// <para>
		/// This is useful for partitioning the screen into separate errors such as the split screen mode common in two player video games.
		/// </para>
		/// </summary>
		/// <param name="x">horizontal pixel position</param>
		/// <param name="y">vertical pixel position</param>
		/// <param name="width">width in pixels</param>
		/// <param name="height">height in pixels</param>
		public static int Viewport(int x, int y, int width, int height) { return bbViewport(x, y, width, height); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbGetColor(int x, int y);
		/// <summary>
		/// <para>
		/// The <c>GetColor</c> command sets the current drawing color
		/// to that of the pixel at the specified screen coordinates.
		/// </para>
		/// <para>
		/// The <c>ColorRed,</c> <c>ColorGreen</c> and <c>ColorBlue</c> functions can
		/// be used to retrieve the current drawing color, allowing you
		/// to determine a pixel's color.
		/// </para>
		/// </summary>
		/// <param name="x">horizontal pixel position</param>
		/// <param name="y">vertical pixel position</param>
		public static int GetColor(int x, int y) { return bbGetColor(x, y); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbColorRed();
		/// <summary>
		/// <para>
		/// The <c>ColorRed</c> function returns the red component of the current
		/// drawing color.
		/// </para>
		/// </summary>
		public static int ColorRed() { return bbColorRed(); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbColorGreen();
		/// <summary>
		/// <para>
		/// The <c>ColorGreen</c> function returns the green component of the current
		/// drawing color.
		/// </para>
		/// </summary>
		public static int ColorGreen() { return bbColorGreen(); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbColorBlue();
		/// <summary>
		/// <para>
		/// The <c>ColorBlue</c> function returns the blue component of the current
		/// drawing color.
		/// </para>
		/// </summary>
		public static int ColorBlue() { return bbColorBlue(); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbText(int x, int y, String str, int centerX, int centerY);
		/// <summary>
		/// <para>
		/// The <c>Text</c> command prints the <c>string</c> specified at the pixel
		/// coordinate <c>x,<c>y.</c></c>
		/// </para>
		/// <para>
		/// <c>Text</c> uses the current font which can be modified with the
		/// <c>SetFont</c> command and the current color which can be modified
		/// with the <c>Color</c> command.
		/// </para>
		/// <para>
		/// The optional centering parameters allow the specified pixel
		/// position to be used as the center of the text printed rather
		/// than representing the top left position of the region.
		/// </para>
		/// </summary>
		/// <param name="x">horizontal pixel position of top left enclosing rectangle</param>
		/// <param name="y">vertical pixel position of the top left enclosing rectangle</param>
		/// <param name="str">string/text to print</param>
		/// <param name="centerX">True to center text horizontally</param>
		/// <param name="centerY">True to center text vertically</param>
		public static int Text(int x, int y, String str, int centerX, int centerY) { return bbText(x, y, str, centerX, centerY); }
		public static int Text(int x, int y, String str, int centerX) { return bbText(x, y, str, centerX, 0); }
		public static int Text(int x, int y, String str) { return bbText(x, y, str, 0, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbLoadFont(String fontname, int height, int bold, int italic, int underlined);
		/// <summary>
		/// <para>
		/// The <c>LoadFont</c> function loads a font and returns a font handle which can
		/// subsequently used with commands such as <c>SetFont</c> and <c>FreeFont.</c>
		/// </para>
		/// </summary>
		/// <param name="fontname">name of font to be loaded, e.g. &quot;arial&quot;</param>
		/// <param name="height">height of font in points (default is 12)</param>
		/// <param name="bold">True to load bold version of font</param>
		/// <param name="italic">True to load italic version of font</param>
		/// <param name="underlined">True to load underlined version of font</param>
		public static int LoadFont(String fontname, int height, int bold, int italic, int underlined) { return bbLoadFont(fontname, height, bold, italic, underlined); }
		public static int LoadFont(String fontname, int height, int bold, int italic) { return bbLoadFont(fontname, height, bold, italic, 0); }
		public static int LoadFont(String fontname, int height, int bold) { return bbLoadFont(fontname, height, bold, 0, 0); }
		public static int LoadFont(String fontname, int height) { return bbLoadFont(fontname, height, 0, 0, 0); }
		public static int LoadFont(String fontname) { return bbLoadFont(fontname, 12, 0, 0, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbSetFont(int fonthandle);
		/// <summary>
		/// <para>
		/// The <c>SetFont</c> command is used in combination with a prior LoadFont
		/// command to specify which font subsequent <c>Text,</c> <c>Print,</c> <c>Write,</c>
		/// <c>FontWidth,</c> <c>FontHeigtht,</c> <c>StringWidth</c> and <c>StringHeight</c> commands 
		/// will use.
		/// </para>
		/// </summary>
		/// <param name="fonthandle">handle of a font successfully returned by <c>LoadFont</c></param>
		public static int SetFont(int fonthandle) { return bbSetFont(fonthandle); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbFreeFont(int fonthandle);
		/// <summary>
		/// <para>
		/// Use the <c>FreeFont</c> command when a font returned by the <c>LoadFont</c>
		/// command is no longer required for text drawing duties.
		/// </para>
		/// </summary>
		/// <param name="fonthandle">A handle to a previously loaded font.</param>
		public static int FreeFont(int fonthandle) { return bbFreeFont(fonthandle); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbFontWidth();
		/// <summary>
		/// <para>
		/// Returns the current width in pixels of the WIDEST character in
		/// the font.
		/// </para>
		/// </summary>
		public static int FontWidth() { return bbFontWidth(); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbFontHeight();
		/// <summary>
		/// <para>
		/// Returns the current height in pixels of the currently selected font.
		/// </para>
		/// </summary>
		public static int FontHeight() { return bbFontHeight(); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbStringWidth(String str);
		/// <summary>
		/// <para>
		/// Returns the width in pixels of the specified string accounting 
		/// for the current font selected with the most recent <c>SetFont</c> 
		/// command for the current graphics buffer.
		/// </para>
		/// </summary>
		/// <param name="str">any valid string or string variable</param>
		public static int StringWidth(String str) { return bbStringWidth(str); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbStringHeight(String str);
		/// <summary>
		/// <para>
		/// Returns the height in pixels of the specified string accounting 
		/// for the current font selected with the most recent <c>SetFont</c> 
		/// command for the current graphics buffer.
		/// </para>
		/// </summary>
		/// <param name="str">any valid string or string variable</param>
		public static int StringHeight(String str) { return bbStringHeight(str); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbLoadImage(String filename);
		/// <summary>
		/// <para>
		/// Reads an image file from disk.
		/// </para>
		/// <para>
		/// Blitz3D supports BMP, JPG and PNG image formats.
		/// </para>
		/// <list type="table">
		/// <listheader><term>Extension</term><term>Compression</term><term>Features</term></listheader>
		/// <item><description>bmp</description></item><item><description> none</description></item><item><description> can be created with <c>SaveImage</c> command</description></item>
		/// <item><description>png</description></item><item><description> good</description></item><item><description> loss-less compression</description></item>
		/// <item><description>jpg</description></item><item><description> excellent</description></item><item><description> small loss in image quality</description></item>
		/// </list>
		/// <para>
		/// The PNG image format is recomended for general use.
		/// </para>
		/// <para>
		/// The <c>LoadImage</c> function returns an image handle that
		/// can then be used with <c>DrawImage</c> to draw the image
		/// on the current graphics buffer.
		/// </para>
		/// <para>
		/// If the image file contains multiple frames of animation
		/// use the <c>LoadAnimImage</c> function instead.
		/// </para>
		/// <para>
		/// If the image file cannot be located or there is a
		/// problem loading, <c>LoadImage</c> will fail and return 0.
		/// </para>
		/// </summary>
		/// <param name="filename">the name of the image file to be loaded</param>
		public static int LoadImage(String filename) { return bbLoadImage(filename); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbLoadAnimImage(String filename, int width, int height, int first, int count);
		/// <summary>
		/// <para>
		/// The <c>LoadAnimImage</c> function is an alternative to 
		/// <c>LoadImage</c> that can load many frames of animation 
		/// from a single image file.
		/// </para>
		/// <para>
		/// The frames must be drawn in similar sized rectangles
		/// arranged from left to right, top to bottom on the
		/// image source.
		/// </para>
		/// <para>
		/// Animation is achieved by selecting a different frame
		/// of animation to be used each time the image is drawn.
		/// The optional <c>frame</c> parameter of commands such as
		/// <c>DrawImage</c> select a specific frame of animation 
		/// to draw of the specified <c>image</c> loaded with this
		/// command.
		/// </para>
		/// <para>
		/// If the image file cannot be located or there is a
		/// problem loading, <c>LoadAnimImage</c> will fail and return 0.
		/// </para>
		/// </summary>
		/// <param name="filename">the name of the image file to be loaded</param>
		/// <param name="width">width in pixels of each frame in the image.</param>
		/// <param name="height">height in pixels of each frame in the image.</param>
		/// <param name="first">index of first animation frame in source image to load(usually 0)</param>
		/// <param name="count">number of frames to load</param>
		public static int LoadAnimImage(String filename, int width, int height, int first, int count) { return bbLoadAnimImage(filename, width, height, first, count); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbCreateImage(int width, int height, int frames);
		/// <summary>
		/// <para>
		/// The <c>CreateImage</c> function returns a new image with the
		/// specified dimensions in pixels containing an optional
		/// number of animation frames.
		/// </para>
		/// <para>
		/// Images need not be loaded from files but can instead be
		/// created and modified by the program. Once an image
		/// is created with <c>CreateImage</c> it can be used as the destination
		/// of a <c>GrabImage</c> command or its pixel buffer can be
		/// accessed directly with the <c>ImageBuffer</c> command.
		/// </para>
		/// </summary>
		/// <param name="width">width in pixels of the new image</param>
		/// <param name="height">height in pixels of the new image</param>
		/// <param name="frames">optional number of frames</param>
		public static int CreateImage(int width, int height, int frames) { return bbCreateImage(width, height, frames); }
		public static int CreateImage(int width, int height) { return bbCreateImage(width, height, 1); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbMaskImage(int image, int red, int green, int blue);
		/// <summary>
		/// <para>
		/// The color specified by mixing the <c>red,</c> <c>green</c> and <c>blue</c>
		/// amounts is assigned as the mask color of the specified
		/// image.
		/// </para>
		/// <para>
		/// When an image is drawn with <c>DrawImage,</c> <c>TileImage</c> 
		/// or <c>DrawImageRect,</c> any pixels in the image that are the
		/// same color as the mask color are not drawn.
		/// </para>
		/// <para>
		/// <c>DrawBlock</c> and other block based commands can be used to 
		/// draw an image and ignore the image's mask color.
		/// </para>
		/// <para>
		/// By default an image has a mask color of black.
		/// </para>
		/// </summary>
		/// <param name="image">a valid image handle</param>
		/// <param name="red">amount of red (0..255)</param>
		/// <param name="green">amount of green (0..255)</param>
		/// <param name="blue">amount of blue (0..255)</param>
		public static int MaskImage(int image, int red, int green, int blue) { return bbMaskImage(image, red, green, blue); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbImageWidth(int image);
		/// <summary>
		/// <para>
		/// Returns the width in pixels of the specified image.
		/// </para>
		/// <para>
		/// Use this function and <c>ImageHeight</c> to ascertain the
		/// exact pixel size of an image's bounding rectangle.
		/// </para>
		/// </summary>
		/// <param name="image">a valid image handle</param>
		public static int ImageWidth(int image) { return bbImageWidth(image); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbImageHeight(int image);
		/// <summary>
		/// <para>
		/// Returns the height in pixels of the specified image.
		/// </para>
		/// <para>
		/// Use this function and <c>ImageWidth</c> to ascertain the
		/// exact pixel size of an image's bounding rectangle.
		/// </para>
		/// </summary>
		/// <param name="image">a valid image handle</param>
		public static int ImageHeight(int image) { return bbImageHeight(image); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbSaveImage(int image, String bmpfile, int frame);
		/// <summary>
		/// <para>
		/// <c>SaveImage</c> saves an image or one of its frames to a .bmp format
		/// image file.
		/// </para>
		/// <para>
		/// Returns <c>True</c> if the save was successful, <c>False</c> if not.
		/// </para>
		/// <para>
		/// The .bmp suffix should be included at the end of the filename if the 
		/// image file created by <c>SaveImage</c> is to be recognized as a valid image 
		/// by the system and other applications. 
		/// </para>
		/// </summary>
		/// <param name="image">a valid image handle</param>
		/// <param name="bmpfile">the filename to be used when the image file is created</param>
		/// <param name="frame">optional frame of the image to save</param>
		public static int SaveImage(int image, String bmpfile, int frame) { return bbSaveImage(image, bmpfile, frame); }
		public static int SaveImage(int image, String bmpfile) { return bbSaveImage(image, bmpfile, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbFreeImage(int image);
		/// <summary>
		/// <para>
		/// The <c>FreeImage</c> command releases all memory used by
		/// the image specified.
		/// </para>
		/// <para>
		/// Following a call to <c>FreeImage</c> the specified image handle
		/// is no longer valid and must not be used.
		/// </para>
		/// </summary>
		/// <param name="image">a valid image handle</param>
		public static int FreeImage(int image) { return bbFreeImage(image); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbDrawImage(int image, int x, int y, int frame);
		/// <summary>
		/// <para>
		/// The <c>DrawImage</c> command draws an image to the current 
		/// graphics buffer at the specified pixel location.
		/// </para>
		/// <para>
		/// The <c>image</c> parameter must be a valid image loaded
		/// with <c>LoadImage</c> or <c>LoadAnimImage</c> or alternatively 
		/// created with <c>CreateImage.</c>
		/// </para>
		/// <para>
		/// If specified, a particular frame of animation from the
		/// image may be drawn. The image in this situation must 
		/// be the result of a call to <c>LoadAnimImage</c> and contain
		/// the <c>frame</c> specified.
		/// </para>
		/// <para>
		/// A faster version of <c>DrawImage</c> is available for images
		/// that do not contain a mask or alpha channel called 
		/// <c>DrawBlock.</c>
		/// </para>
		/// </summary>
		/// <param name="image">a valid image handle</param>
		/// <param name="x">horizontal pixel location</param>
		/// <param name="y">vertical pixel location</param>
		/// <param name="frame">optional frame number</param>
		public static int DrawImage(int image, int x, int y, int frame) { return bbDrawImage(image, x, y, frame); }
		public static int DrawImage(int image, int x, int y) { return bbDrawImage(image, x, y, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbDrawBlock(int image, int x, int y, int frame);
		/// <summary>
		/// <para>
		/// <c>DrawBlock</c> is similar to <c>DrawImage</c> except all masking
		/// and image transparency is ignored.
		/// </para>
		/// </summary>
		/// <param name="image">a valid image handle</param>
		/// <param name="x">horizontal pixel location</param>
		/// <param name="y">vertical pixel location</param>
		/// <param name="frame">optional frame number</param>
		public static int DrawBlock(int image, int x, int y, int frame) { return bbDrawBlock(image, x, y, frame); }
		public static int DrawBlock(int image, int x, int y) { return bbDrawBlock(image, x, y, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbDrawImageRect(int image, int x, int y, int image_x, int image_y, int width, int height, int frame);
		/// <summary>
		/// <para>
		/// The <c>DrawImageRect</c> command draws a part of an Image on
		/// to the current graphics buffer at location <c>x,</c> <c>y.</c> 
		/// </para>
		/// <para>
		/// The region of the image used is defined by the rectangle
		/// at location <c>image_x,<c>image_y</c></c> of size <c>width,<c>height.</c></c>
		/// </para>
		/// <para>
		/// See <c>DrawImage</c> for more details about drawing with images.
		/// </para>
		/// </summary>
		/// <param name="image">a valid image handle</param>
		/// <param name="x">horizontal pixel location</param>
		/// <param name="y">vertical pixel location</param>
		/// <param name="image_x">horizontal pixel location in image</param>
		/// <param name="image_y">vertical pixel location in image</param>
		/// <param name="width">width of rectangle to Draw</param>
		/// <param name="height">height of rectangle to Draw</param>
		/// <param name="frame">optional frame number</param>
		public static int DrawImageRect(int image, int x, int y, int image_x, int image_y, int width, int height, int frame) { return bbDrawImageRect(image, x, y, image_x, image_y, width, height, frame); }
		public static int DrawImageRect(int image, int x, int y, int image_x, int image_y, int width, int height) { return bbDrawImageRect(image, x, y, image_x, image_y, width, height, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbDrawBlockRect(int image, int x, int y, int image_x, int image_y, int width, int height, int frame);
		/// <summary>
		/// <para>
		/// The <c>DrawBlockRect</c> command is similar to <c>DrawImageRect</c>
		/// but ignores any masking and transparency in the source
		/// image.
		/// </para>
		/// </summary>
		/// <param name="image">a valid image handle</param>
		/// <param name="x">horizontal pixel location</param>
		/// <param name="y">vertical pixel location</param>
		/// <param name="image_x">horizontal pixel location in image</param>
		/// <param name="image_y">vertical pixel location in image</param>
		/// <param name="width">width of rectangle to Draw</param>
		/// <param name="height">height of rectangle to Draw</param>
		/// <param name="frame">optional frame number</param>
		public static int DrawBlockRect(int image, int x, int y, int image_x, int image_y, int width, int height, int frame) { return bbDrawBlockRect(image, x, y, image_x, image_y, width, height, frame); }
		public static int DrawBlockRect(int image, int x, int y, int image_x, int image_y, int width, int height) { return bbDrawBlockRect(image, x, y, image_x, image_y, width, height, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbTileImage(int image, int x, int y, int frame);
		/// <summary>
		/// <para>
		/// The <c>TileImage</c> command tiles the entire viewport 
		/// of the current graphics buffer with the specified
		/// image.
		/// </para>
		/// <para>
		/// The optional pixel offsets effectively scroll the
		/// tilemap drawn in the direction specified.
		/// </para>
		/// <para>
		/// See <c>DrawImage</c> for more drawing images details.
		/// </para>
		/// </summary>
		/// <param name="image">a valid image handle</param>
		/// <param name="x">horizontal pixel offset</param>
		/// <param name="y">vertical pixel offset</param>
		/// <param name="frame">optional frame number</param>
		public static int TileImage(int image, int x, int y, int frame) { return bbTileImage(image, x, y, frame); }
		public static int TileImage(int image, int x, int y) { return bbTileImage(image, x, y, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbTileBlock(int image, int x, int y, int frame);
		/// <summary>
		/// <para>
		/// Similar to <c>TileImage</c> but ignores transparency.
		/// </para>
		/// <para>
		/// Use this to tile an entire or portion of the screen 
		/// with a single repetitive image.
		/// </para>
		/// </summary>
		/// <param name="image">a valid image handle</param>
		/// <param name="x">horizontal pixel offset</param>
		/// <param name="y">vertical pixel offset</param>
		/// <param name="frame">optional frame number</param>
		public static int TileBlock(int image, int x, int y, int frame) { return bbTileBlock(image, x, y, frame); }
		public static int TileBlock(int image, int x, int y) { return bbTileBlock(image, x, y, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbHandleImage(int image, int x, int y);
		/// <summary>
		/// <para>
		/// Sets an image's drawing handle to the specified pixel
		/// offset.
		/// </para>
		/// <para>
		/// An image's handle is an offset added to the pixel
		/// coordinate specified in a <c>DrawImage</c> command.
		/// </para>
		/// <para>
		/// Images typically have their handle set to 0,0 which means
		/// drawing commands draw the image with its top left pixel
		/// at the drawing location specified.
		/// </para>
		/// <para>
		/// The <c>AutoMidHandle</c> command changes this behavior so that
		/// all subsequent Images are loaded or created with their
		/// handle set to the center of the Image.
		/// </para>
		/// <para>
		/// The <c>HandleImage</c> command is used to position the handle
		/// to any given pixel offset after it has been created.
		/// </para>
		/// <para>
		/// Also See:
		/// MidHandle;AutoMidHandle;DrawImage;RotateImage
		/// </para>
		/// </summary>
		/// <param name="image">a valid image handle</param>
		/// <param name="x">horizontal pixel offset</param>
		/// <param name="y">vertical pixel offset</param>
		public static int HandleImage(int image, int x, int y) { return bbHandleImage(image, x, y); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbMidHandle(int image);
		/// <summary>
		/// <para>
		/// The <c>MidHandle</c> command sets the specified image's handle
		/// to the center of the image. See <c>HandleImage</c> for more
		/// details on using image handles.
		/// </para>
		/// </summary>
		/// <param name="image">a valid image handle</param>
		public static int MidHandle(int image) { return bbMidHandle(image); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbAutoMidHandle(int enable);
		/// <summary>
		/// <para>
		/// Enabling <c>AutoMidHandle</c> causes all subsequent loaded and created
		/// images to have their handles initialized to the center of the image.
		/// </para>
		/// <para>
		/// The default setting of the AutoMidHandle setting is disabled
		/// which dictates all newly create images have their handles set
		/// to the top left pixel position of the image.
		/// </para>
		/// </summary>
		/// <param name="enable">True to enable automtic MidHandles, False to disable</param>
		public static int AutoMidHandle(int enable) { return bbAutoMidHandle(enable); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbImageXHandle(int image);
		/// <summary>
		/// <para>
		/// Returns the horizontal pixel position of an image's handle.
		/// </para>
		/// </summary>
		/// <param name="image">a valid image handle</param>
		public static int ImageXHandle(int image) { return bbImageXHandle(image); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbImageYHandle(int image);
		/// <summary>
		/// <para>
		/// Returns the vertical pixel position of an image's handle.
		/// </para>
		/// </summary>
		/// <param name="image">a valid image handle</param>
		public static int ImageYHandle(int image) { return bbImageYHandle(image); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbCopyImage(int image);
		/// <summary>
		/// <para>
		/// Returns an identical copy of the specified image.
		/// </para>
		/// </summary>
		/// <param name="image">a valid image handle</param>
		public static int CopyImage(int image) { return bbCopyImage(image); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbGrabImage(int image, int x, int y, int frame);
		/// <summary>
		/// <para>
		/// Copies pixels at the specified offset in the current 
		/// graphics buffer to the image specified.
		/// </para>
		/// <para>
		/// <c>GrabImage</c> is a useful way of capturing the result of 
		/// a sequence of drawing commands in an image's pixel
		/// buffer.
		/// </para>
		/// </summary>
		/// <param name="image">a valid image handle</param>
		/// <param name="x">left most horizontal pixel position to grab from</param>
		/// <param name="y">top most vertical pixel position to grab from</param>
		/// <param name="frame">optional frame in which to store grabbed pixels</param>
		public static int GrabImage(int image, int x, int y, int frame) { return bbGrabImage(image, x, y, frame); }
		public static int GrabImage(int image, int x, int y) { return bbGrabImage(image, x, y, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbImageBuffer(int image, int frame);
		/// <summary>
		/// <para>
		/// The <c>ImageBuffer</c> function returns a graphics buffer that
		/// can be used with such commands as <c>SetBuffer</c> and <c>LockBuffer.</c>
		/// </para>
		/// </summary>
		/// <param name="image">a valid image handle</param>
		/// <param name="frame">optional animation frame</param>
		public static int ImageBuffer(int image, int frame) { return bbImageBuffer(image, frame); }
		public static int ImageBuffer(int image) { return bbImageBuffer(image, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbScaleImage(int image, float xscale, float yscale);
		/// <summary>
		/// <para>
		/// The <c>ScaleImage</c> function returns a copy of an image 
		/// scaled in each axis by the specified factors.
		/// </para>
		/// <para>
		/// A negative scale factor also causes the resulting image to be
		/// flipped in that axis, i.e. ScaleImage image,1,-1 will return
		/// a copy of image flipped vertically. Other common scale
		/// factors are 2,2 which produce a double sized image and
		/// 0.5,0.5 which will produce an image half the size of the
		/// original.
		/// </para>
		/// <para>
		/// The quality of the transformed result can be controlled
		/// with the <c>TFormFilter</c> command.
		/// </para>
		/// <para>
		/// See the <c>ResizeImage</c> command for a similar command that
		/// uses a target image size to calculate scale factors.
		/// </para>
		/// <para>
		/// This command is not particularly fast and hence like loading
		/// it is recomended images are scaled before a game level 
		/// commences.
		/// </para>
		/// </summary>
		/// <param name="image">a valid image handle</param>
		/// <param name="xscale">horizontal scale factor</param>
		/// <param name="yscale">vertical scale factor</param>
		public static int ScaleImage(int image, float xscale, float yscale) { return bbScaleImage(image, xscale, yscale); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbResizeImage(int image, float width, float height);
		/// <summary>
		/// <para>
		/// The <c>ResizeImage</c> function returns a copy of the
		/// specified image scaled to the specified pixel
		/// dimensions.
		/// </para>
		/// <para>
		/// This command is not particularly fast and hence like loading
		/// it is recomended images are sized before a game level 
		/// commences.
		/// </para>
		/// </summary>
		/// <param name="image">a valid image handle</param>
		/// <param name="width">horizontal pixel size of new image</param>
		/// <param name="height">vertical pixel size of new image</param>
		public static int ResizeImage(int image, float width, float height) { return bbResizeImage(image, width, height); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbRotateImage(int image, float angle);
		/// <summary>
		/// <para>
		/// The RotateImage function creates a new image by copying
		/// the specified image and rotating it <c>angle</c> degrees around
		/// its current handle.
		/// </para>
		/// <para>
		/// This command is not particularly fast and hence like loading
		/// it is recomended images are prerotated before a game level 
		/// commences.
		/// </para>
		/// </summary>
		/// <param name="image">a valid image handle</param>
		/// <param name="angle">angle in degree to rotate the image clockwise</param>
		public static int RotateImage(int image, float angle) { return bbRotateImage(image, angle); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbTFormImage(int image, float m11, float m12, float m21, float m22);
		/// <summary>
		/// <para>
		/// The <c>TFormImage</c> function is similar in function to the
		/// <c>ScaleImage</c> and <c>RotateImage</c> functions where the image
		/// returned is a transformed copy of the original image 
		/// </para>
		/// <para>
		/// Instead of using a scale factor or angle of rotation,
		/// <c>TFormImage</c> accepts 4 values that define a 2x2 matrix
		/// that allows the resultant copy to be the product of
		/// a transform that is a combination of both scale and
		/// rotation.
		/// </para>
		/// <para>
		/// <c>TFormImage</c> is also useful for shearing an Image.
		/// </para>
		/// <para>
		/// This command is not particularly fast and hence like loading
		/// it is recomended images are transformed before a game level 
		/// commences.
		/// </para>
		/// </summary>
		/// <param name="image">a valid image handle</param>
		/// <param name="m11">first element of 2x2 matrix</param>
		/// <param name="m12">second element of 2x2 matrix</param>
		/// <param name="m21">third element of 2x2 matrix</param>
		/// <param name="m22">fourth element of 2x2 matrix</param>
		public static int TFormImage(int image, float m11, float m12, float m21, float m22) { return bbTFormImage(image, m11, m12, m21, m22); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbTFormFilter(int enable);
		/// <summary>
		/// <para>
		/// The <c>TFormFilter</c> command controls the quality of transformation
		/// achieved when using the <c>ScaleImage,</c> <c>RotateImage</c> and <c>TFormImage</c>
		/// commands.
		/// </para>
		/// <para>
		/// Use a paramter of True to enable filtering, which although
		/// slower produces a higher quality result.
		/// </para>
		/// </summary>
		/// <param name="enable">True to enable filtering, False to disable</param>
		public static int TFormFilter(int enable) { return bbTFormFilter(enable); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbRectsOverlap(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
		/// <summary>
		/// <para>
		/// <c>RectsOverlap</c> returns True if the two rectangular regions
		/// described overlap.
		/// </para>
		/// </summary>
		/// <param name="x1">top left horizontal position of first rectangle</param>
		/// <param name="y1">top left vertical position of first rectangle</param>
		/// <param name="w1">width of first rectangle</param>
		/// <param name="h1">height of first rectangle</param>
		/// <param name="x2">top left horizontal position of second rectangle</param>
		/// <param name="y2">top left vertical position of seconf rectangle</param>
		/// <param name="w2">width of second rectangle</param>
		/// <param name="h2">height of second rectangle</param>
		public static int RectsOverlap(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) { return bbRectsOverlap(x1, y1, w1, h1, x2, y2, w2, h2); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbImagesOverlap(int image1, int x1, int y1, int image2, int x2, int y2);
		/// <summary>
		/// <para>
		/// The <c>ImagesOverlap</c> function returns True if image1 drawn at the 
		/// specified pixel location would overlap with image2 if drawn at 
		/// its specified location.
		/// </para>
		/// <para>
		/// <c>ImagesOverlap</c> does not take into account any transparent pixels and 
		/// hence is faster but less accurate than the comparable <c>ImagesCollide</c> 
		/// function.
		/// </para>
		/// </summary>
		/// <param name="image1">first image to test</param>
		/// <param name="x1">image1's x location</param>
		/// <param name="y1">image1's y location</param>
		/// <param name="image2">second image to test</param>
		/// <param name="x2">image2's x location</param>
		/// <param name="y2">image2's y location</param>
		public static int ImagesOverlap(int image1, int x1, int y1, int image2, int x2, int y2) { return bbImagesOverlap(image1, x1, y1, image2, x2, y2); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbImagesCollide(int image1, int x1, int y1, int frame1, int image2, int x2, int y2, int frame2);
		/// <summary>
		/// <para>
		/// Unlike <c>ImagesOverlap,</c> <c>ImagesCollide</c> does respect transparent
		/// pixels in the source images and will only return True if actual
		/// solid pixels would overlap if the images were drawn in the
		/// specified locations.
		/// </para>
		/// <para>
		/// As <c>ImagesCollide</c> tests actual pixels from the two images it 
		/// is slower but more exact than the <c>ImagesOverlap</c> function.
		/// </para>
		/// </summary>
		/// <param name="image1">first image to test</param>
		/// <param name="x1">image1's x location</param>
		/// <param name="y1">image1's y location</param>
		/// <param name="frame1">image1's frame to test (optional)</param>
		/// <param name="image2">second image to test</param>
		/// <param name="x2">image2's x location</param>
		/// <param name="y2">image2's y location</param>
		/// <param name="frame2">image2's frame to test (optional)</param>
		public static int ImagesCollide(int image1, int x1, int y1, int frame1, int image2, int x2, int y2, int frame2) { return bbImagesCollide(image1, x1, y1, frame1, image2, x2, y2, frame2); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbImageRectOverlap(int image, int x, int y, int rectx, int recty, int rectw, int recth);
		/// <summary>
		/// <para>
		/// The <c>ImageRectOverlap</c> function returns True if the image specified
		/// drawn at the location specified would overlap with the rectangle
		/// described.
		/// </para>
		/// <para>
		/// <c>ImageRectOverlap</c> perform a fast rectangular based test ignoring
		/// the shape of any image mask, see <c>ImageRectCollide</c> for a more
		/// exact collision test.
		/// </para>
		/// </summary>
		/// <param name="image">a valid image handle</param>
		/// <param name="x">horizontal pixel location of image</param>
		/// <param name="y">vertical pixel location of image</param>
		/// <param name="rectx">horizontal pixel location of rect</param>
		/// <param name="recty">vertical pixel location of rect</param>
		/// <param name="rectw">width of the rect</param>
		/// <param name="recth">height of the rect</param>
		public static int ImageRectOverlap(int image, int x, int y, int rectx, int recty, int rectw, int recth) { return bbImageRectOverlap(image, x, y, rectx, recty, rectw, recth); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbImageRectCollide(int image, int x, int y, int frame, int rectx, int recty, int rectw, int recth);
		/// <summary>
		/// <para>
		/// The <c>ImageRectCollide</c> function returns True if the image specified
		/// drawn at the location specified will result in any non transparent
		/// pixels being drawn inside the rectangle described.
		/// </para>
		/// <para>
		/// Because <c>ImageRectCollide</c> respects the transparent pixels in an
		/// image's mask it is slower but more accurate than using the 
		/// <c>ImageRectOverlap</c> command.
		/// </para>
		/// </summary>
		/// <param name="image">a valid image handle</param>
		/// <param name="x">horizontal pixel location of image</param>
		/// <param name="y">vertical pixel location of image</param>
		/// <param name="frame">image's frame</param>
		/// <param name="rectx">horizontal pixel location of rect</param>
		/// <param name="recty">vertical pixel location of rect</param>
		/// <param name="rectw">width of the rect</param>
		/// <param name="recth">height of the rect</param>
		public static int ImageRectCollide(int image, int x, int y, int frame, int rectx, int recty, int rectw, int recth) { return bbImageRectCollide(image, x, y, frame, rectx, recty, rectw, recth); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbSetBuffer(int buffer);
		/// <summary>
		/// <para>
		/// The <c>SetBuffer</c> command is used to set the current graphics buffer.
		/// </para>
		/// <para>
		/// After calling <c>SetBuffer,</c> All 2D rendering commands will write to the specified
		/// graphics buffer. 3D rendering always writes to the back buffer.
		/// </para>
		/// <para>
		/// <c>SetBuffer</c> also resets the <c>Origin</c> to 0,0 and the <c>Viewport</c> to the
		/// dimensions of the entire selected buffer.
		/// </para>
		/// <para>
		/// <c>buffer</c> should be a valid graphics buffer returned by <c>FrontBuffer,</c> 
		/// <c>BackBuffer,</c> <c>ImageBuffer,</c> <c>TextureBuffer</c> or the result of a previous call 
		/// to <c>GraphicsBuffer.</c>
		/// </para>
		/// <para>
		/// At the beginning of program execution and following any call
		/// to <c>Graphics</c> the current graphics buffer is set to the display's
		/// <c>FrontBuffer.</c>
		/// </para>
		/// <para>
		/// After a call to <c>Graphics3D</c> the current buffer is set to the
		/// display's <c>BackBuffer.</c>
		/// </para>
		/// </summary>
		/// <param name="buffer">a valid graphics buffer</param>
		public static int SetBuffer(int buffer) { return bbSetBuffer(buffer); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbGraphicsBuffer();
		/// <summary>
		/// <para>
		/// Returns the currently selected graphics buffer.
		/// </para>
		/// <para>
		/// The <c>GraphicsBuffer</c> function is useful for storing the current
		/// graphics buffer so it can be restored later.
		/// </para>
		/// <para>
		/// See <c>SetBuffer</c> for more information.
		/// </para>
		/// </summary>
		public static int GraphicsBuffer() { return bbGraphicsBuffer(); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbLoadBuffer(int buffer, String filename);
		/// <summary>
		/// <para>
		/// Instead of calling <c>LoadImage</c> and creating a new image
		/// the <c>LoadBuffer</c> command reads the contents of a valid image
		/// file into the contents of an existing image, texture
		/// or if required the front or back buffer of the current
		/// display.
		/// </para>
		/// </summary>
		/// <param name="buffer">a valid graphics buffer</param>
		/// <param name="filename">the filename of an existing image file</param>
		public static int LoadBuffer(int buffer, String filename) { return bbLoadBuffer(buffer, filename); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbSaveBuffer(int buffer, String filename);
		/// <summary>
		/// <para>
		/// The <c>SaveBuffer</c> function is similar to the <c>SaveImage</c> function
		/// in that it creates a .bmp image file with the specified <c>filename.</c>
		/// </para>
		/// <para>
		/// Unlike <c>SaveImage,</c> <c>SaveBuffer</c> uses the pixels from the specified
		/// graphics buffer and so is useful for making screenshots.
		/// </para>
		/// <para>
		/// The .bmp suffix should be included at the end of the filename if the 
		/// image file created by <c>SaveBuffer</c> is to be recognized as a valid image 
		/// by the system and other applications. 
		/// </para>
		/// </summary>
		/// <param name="buffer">a valid graphics buffer</param>
		/// <param name="filename">a legal filename</param>
		public static int SaveBuffer(int buffer, String filename) { return bbSaveBuffer(buffer, filename); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbReadPixel(int x, int y, int buffer);
		/// <summary>
		/// <para>
		/// The <c>ReadPixel</c> function determines the color of a pixel at the
		/// specified location of the specified graphics buffer.
		/// </para>
		/// <para>
		/// The return value is an integer with the red, green and blue
		/// values packed int the low 24 binary bits and a transparency
		/// value in the high 8 bits.
		/// </para>
		/// <para>
		/// If the x,y coordinate falls outside the bounds of the buffer
		/// a value of BLACK or in the case of an image buffer, the images
		/// mask color is returned.
		/// </para>
		/// <para>
		/// If no graphics buffer is specified <c>ReadPixel</c> uses the current
		/// graphics buffer, see <c>SetBuffer</c> for more details.
		/// </para>
		/// </summary>
		/// <param name="x">horizontal pixel location</param>
		/// <param name="y">vertical pixel location</param>
		/// <param name="buffer">valid graphics buffer</param>
		public static int ReadPixel(int x, int y, int buffer) { return bbReadPixel(x, y, buffer); }
		public static int ReadPixel(int x, int y) { return bbReadPixel(x, y, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbWritePixel(int x, int y, int color, int buffer);
		/// <summary>
		/// <para>
		/// The <c>WritePixel</c> command sets the color of a pixel at the
		/// specified location of the specified graphics buffer to the
		/// value <c>color.</c>
		/// </para>
		/// <para>
		/// The <c>color</c> value is an integer with the red, green and blue
		/// values packed into the low 24 binary bits and if required
		/// the transparency value in the high 8 bits.
		/// </para>
		/// <para>
		/// If the x,y coordinate falls outside the bounds of the buffer
		/// the <c>WritePixel</c> command does nothing.
		/// </para>
		/// <para>
		/// If no graphics buffer is specified <c>WritePixel</c> uses the current
		/// graphics buffer, see <c>SetBuffer</c> for more details.
		/// </para>
		/// </summary>
		/// <param name="x">horizontal pixel location</param>
		/// <param name="y">vertical pixel location</param>
		/// <param name="color">binary packed color value</param>
		/// <param name="buffer">valid graphics buffer</param>
		public static int WritePixel(int x, int y, int color, int buffer) { return bbWritePixel(x, y, color, buffer); }
		public static int WritePixel(int x, int y, int color) { return bbWritePixel(x, y, color, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbCopyPixel(int src_x, int src_y, int src_buffer, int dest_x, int dest_y, int dest_buffer);
		/// <summary>
		/// <para>
		/// The <c>CopyPixel</c> command sets the color of a pixel at the
		/// destination location of the destination graphics buffer 
		/// to the color of the pixel at the source location of
		/// the source buffer.
		/// </para>
		/// <para>
		/// If no destination graphics buffer is specified <c>CopyPixel</c> 
		/// writes to the the current graphics buffer.
		/// </para>
		/// </summary>
		/// <param name="src_x">horizontal pixel location</param>
		/// <param name="src_y">vertical pixel location</param>
		/// <param name="src_buffer">valid graphics buffer to read from</param>
		/// <param name="dest_x">horizontal pixel location</param>
		/// <param name="dest_y">vertical pixel location</param>
		/// <param name="dest_buffer">valid graphics buffer to write to</param>
		public static int CopyPixel(int src_x, int src_y, int src_buffer, int dest_x, int dest_y, int dest_buffer) { return bbCopyPixel(src_x, src_y, src_buffer, dest_x, dest_y, dest_buffer); }
		public static int CopyPixel(int src_x, int src_y, int src_buffer, int dest_x, int dest_y) { return bbCopyPixel(src_x, src_y, src_buffer, dest_x, dest_y, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbCopyRect(int src_x, int src_y, int width, int height, int dest_x, int dest_y, int src_buffer, int dest_buffer);
		/// <summary>
		/// <para>
		/// The <c>CopyRect</c> command is similar to <c>CopyPixel</c> but copies
		/// a region of pixels <c>width,</c> <c>height</c> in size.
		/// </para>
		/// <para>
		/// If src_buffer and dest_buffer are not specified <c>CopyRect</c> uses
		/// the current graphics buffer, see <c>SetBuffer</c> for more details.
		/// </para>
		/// </summary>
		/// <param name="src_x">horizontal pixel location</param>
		/// <param name="src_y">vertical pixel location</param>
		/// <param name="width">horizontal size of pixel region to copy</param>
		/// <param name="height">vertical size of pixel region to copy</param>
		/// <param name="dest_x">horizontal destination pixel location</param>
		/// <param name="dest_y">vertical destination pixel location</param>
		/// <param name="src_buffer">valid graphics buffer</param>
		/// <param name="dest_buffer">valid graphics buffer</param>
		public static int CopyRect(int src_x, int src_y, int width, int height, int dest_x, int dest_y, int src_buffer, int dest_buffer) { return bbCopyRect(src_x, src_y, width, height, dest_x, dest_y, src_buffer, dest_buffer); }
		public static int CopyRect(int src_x, int src_y, int width, int height, int dest_x, int dest_y, int src_buffer) { return bbCopyRect(src_x, src_y, width, height, dest_x, dest_y, src_buffer, 0); }
		public static int CopyRect(int src_x, int src_y, int width, int height, int dest_x, int dest_y) { return bbCopyRect(src_x, src_y, width, height, dest_x, dest_y, 0, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbLockBuffer(int buffer);
		/// <summary>
		/// <para>
		/// <c>LockBuffer</c> locks the specified graphics buffer.
		/// </para>
		/// <para>
		/// High speed pixel functions such as <c>ReadPixelFast,</c> <c>WritePixelFast</c> 
		/// and <c>CopyPixelFast</c> require the graphics buffer be in a locked state.
		/// </para>
		/// <para>
		/// <c>UnlockBuffer</c> must be used once the high speed pixel manipulation
		/// is complete.
		/// </para>
		/// <para>
		/// Standard drawing commands should not be issued when a buffer is in
		/// a locked state.
		/// </para>
		/// <para>
		/// See the other commands for more
		/// information.
		/// </para>
		/// </summary>
		/// <param name="buffer">any valid graphics buffer</param>
		public static int LockBuffer(int buffer) { return bbLockBuffer(buffer); }
		public static int LockBuffer() { return bbLockBuffer(0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbUnlockBuffer(int buffer);
		/// <summary>
		/// <para>
		/// <c>UnlockBuffer</c> must be used once the high speed pixel manipulation
		/// is complete on a locked buffer.
		/// </para>
		/// <para>
		/// See <c>LockBuffer</c> for more information.
		/// </para>
		/// </summary>
		/// <param name="buffer">any valid locked graphics buffer</param>
		public static int UnlockBuffer(int buffer) { return bbUnlockBuffer(buffer); }
		public static int UnlockBuffer() { return bbUnlockBuffer(0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbReadPixelFast(int x, int y, int buffer);
		/// <summary>
		/// <para>
		/// <c>ReadPixelFast</c> is similar in function to <c>ReadPixel</c> but the buffer
		/// must be locked with the <c>LockBuffer</c> command and no bounds checking 
		/// is performed in the interests of speed.
		/// </para>
		/// <para>
		/// Warning: like <c>WritePixelFast</c> extreme care must be taken to ensure
		/// the pixel position specified falls inside the specified buffers
		/// area to avoid crashing the computer.
		/// </para>
		/// </summary>
		/// <param name="x">horizontal pixel location</param>
		/// <param name="y">vertical pixel location</param>
		/// <param name="buffer">valid graphics buffer</param>
		public static int ReadPixelFast(int x, int y, int buffer) { return bbReadPixelFast(x, y, buffer); }
		public static int ReadPixelFast(int x, int y) { return bbReadPixelFast(x, y, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbWritePixelFast(int x, int y, int color, int buffer);
		/// <summary>
		/// <para>
		/// <c>WritePixelFast</c> is similar in function to <c>ReadPixel</c> but the buffer
		/// must be locked with the <c>LockBuffer</c> command and no bounds checking 
		/// is performed in the interests of speed.
		/// </para>
		/// <para>
		/// Warning: like <c>ReadPixelFast</c> extreme care must be taken to ensure
		/// the pixel position specified falls inside the specified buffers
		/// area to avoid crashing the computer.
		/// </para>
		/// </summary>
		/// <param name="x">horizontal pixel location</param>
		/// <param name="y">vertical pixel location</param>
		/// <param name="color">binary packed color value</param>
		/// <param name="buffer">valid graphics buffer</param>
		public static int WritePixelFast(int x, int y, int color, int buffer) { return bbWritePixelFast(x, y, color, buffer); }
		public static int WritePixelFast(int x, int y, int color) { return bbWritePixelFast(x, y, color, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbCopyPixelFast(int src_x, int src_y, int src_buffer, int dest_x, int dest_y, int dest_buffer);
		/// <summary>
		/// <para>
		/// <c>CopyPixelFast</c> is similar in function to <c>CopyPixel</c> but the buffer
		/// must be locked with the <c>LockBuffer</c> command and no bounds checking 
		/// is performed in the interests of speed.
		/// </para>
		/// <para>
		/// Warning: like <c>ReadPixelFast</c> extreme care must be taken to ensure
		/// the pixel position specified falls inside the specified buffers
		/// area to avoid crashing the computer.
		/// </para>
		/// </summary>
		/// <param name="src_x">horizontal pixel location</param>
		/// <param name="src_y">vertical pixel location</param>
		/// <param name="src_buffer">valid graphics buffer to read from</param>
		/// <param name="dest_x">horizontal pixel location</param>
		/// <param name="dest_y">vertical pixel location</param>
		/// <param name="dest_buffer">valid graphics buffer to write to</param>
		public static int CopyPixelFast(int src_x, int src_y, int src_buffer, int dest_x, int dest_y, int dest_buffer) { return bbCopyPixelFast(src_x, src_y, src_buffer, dest_x, dest_y, dest_buffer); }
		public static int CopyPixelFast(int src_x, int src_y, int src_buffer, int dest_x, int dest_y) { return bbCopyPixelFast(src_x, src_y, src_buffer, dest_x, dest_y, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbCountGfxModes();
		/// <summary>
		/// <para>
		/// The <c>CountGfxModes</c> function returns the number of graphics modes
		/// supported by the current graphics driver.
		/// </para>
		/// <para>
		/// Use the <c>GfxModeWidth,</c> <c>GfxModeHeight</c> and <c>GfxModeDepth</c> to obtain
		/// information about each mode for use with the <c>Graphics</c> command.
		/// </para>
		/// <para>
		/// Legal graphics modes for these functions are numbered from 1 up 
		/// to and including the value returned by <c>CountGfxModes.</c>
		/// </para>
		/// <para>
		/// Use <c>CountGfxModes3D()</c> instead if <c>Graphics3D</c> use is required as older
		/// hardware may support 3D acceleration on only a subset of its video modes.
		/// </para>
		/// </summary>
		public static int CountGfxModes() { return bbCountGfxModes(); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbGfxModeWidth(int mode);
		/// <summary>
		/// <para>
		/// Returns the width in pixels of the specified graphics mode where mode
		/// is a value from 1 up to and including the result of the CountGfxModes()
		/// Function.
		/// </para>
		/// </summary>
		public static int GfxModeWidth(int mode) { return bbGfxModeWidth(mode); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbGfxModeHeight(int mode);
		/// <summary>
		/// <para>
		/// Returns the height in pixels of the specified graphics mode where mode
		/// is a value from 1 up to and including the result of the CountGfxModes()
		/// Function.
		/// </para>
		/// </summary>
		public static int GfxModeHeight(int mode) { return bbGfxModeHeight(mode); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbGfxModeDepth(int mode);
		/// <summary>
		/// <para>
		/// Returns the depth in pixels of the specified graphics mode where mode
		/// is a value from 1 up to and including the result of the CountGfxModes()
		/// Function.
		/// </para>
		/// </summary>
		public static int GfxModeDepth(int mode) { return bbGfxModeDepth(mode); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbGfxModeExists(int width, int height, int depth);
		/// <summary>
		/// <para>
		/// Returns True if the resolution specified is supported by the
		/// current graphics driver. Calling <c>Graphics</c> with settings that
		/// cause this function to return <c>False</c> will cause your program
		/// to halt.
		/// </para>
		/// <para>
		/// For more information see <c>Graphics.</c>
		/// </para>
		/// <para>
		/// For an alternative method for verifying legal resolutions
		/// see the <c>CountGfxModes</c> function.
		/// </para>
		/// </summary>
		/// <param name="width">width in pixels</param>
		/// <param name="height">height in pixels</param>
		/// <param name="depth">color depth in bits</param>
		public static int GfxModeExists(int width, int height, int depth) { return bbGfxModeExists(width, height, depth); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbCountGfxDrivers();
		/// <summary>
		/// <para>
		/// <c>CountGfxDrivers</c> returns the number of graphcis drivers 
		/// connected to the system.
		/// </para>
		/// <para>
		/// The <c>SetGfxDriver</c> command is used to change the current graphics driver.
		/// </para>
		/// <para>
		/// A return value of larger than 1 means a secondary monitor is present
		/// and your program may wish to give the user an option for it to be used
		/// for the purposes of playing your game.
		/// </para>
		/// </summary>
		public static int CountGfxDrivers() { return bbCountGfxDrivers(); }

		[DllImport("Blitz3D.dll")]
		public static extern String bbGfxDriverName(int index);
		/// <summary>
		/// <para>
		/// The <c>GfxDriverName</c> function returns the name of the graphics driver
		/// at the specified index.
		/// </para>
		/// <para>
		/// The index parameter should be in the range of 1 up to and
		/// including the value returned by <c>CountGfxDrivers.</c>
		/// </para>
		/// </summary>
		/// <param name="index">index number of display device</param>
		public static String GfxDriverName(int index) { return bbGfxDriverName(index); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbSetGfxDriver(int index);
		/// <summary>
		/// <para>
		/// The <c>SetGfxDriver</c> command is used to change the current graphics driver.
		/// </para>
		/// <para>
		/// The current graphics driver dictates which display device is used on 
		/// a multimonitor system when the <c>Graphics</c> command is used. It also
		/// affects the graphics modes reported by <c>CountGfxModes.</c>
		/// </para>
		/// <para>
		/// The index parameter should be in the range of 1 up to and
		/// including the value returned by <c>CountGfxDrivers.</c>
		/// </para>
		/// </summary>
		/// <param name="index">index number of display device</param>
		public static int SetGfxDriver(int index) { return bbSetGfxDriver(index); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbLoadSound(String filename);
		/// <summary>
		/// <para>
		/// If successful returns the handle of a sound object to be
		/// used with the <c>PlaySound</c> command.
		/// </para>
		/// <para>
		/// The following file formats are supported:
		/// </para>
		/// <list type="table">
		/// <listheader><term>Format</term><term>Compression</term><term>Features</term></listheader>
		/// <item><description>wav</description></item><item><description>none</description></item><item><description>fast loading</description></item>
		/// <item><description>mp3</description></item><item><description>yes</description></item><item><description>license required</description></item>
		/// <item><description>ogg</description></item><item><description>yes</description></item><item><description>license free</description></item>
		/// </list>
		/// <para>
		/// The reader should be aware that an additional license is 
		/// required to distribute software that utilizes playback of 
		/// mp3 files.
		/// </para>
		/// </summary>
		/// <param name="filename">the name of an existing sound file</param>
		public static int LoadSound(String filename) { return bbLoadSound(filename); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbPlaySound(int sound);
		/// <summary>
		/// <para>
		/// Returns the channel allocated for playback.
		/// </para>
		/// <para>
		/// <c>PlaySound</c> plays a sound previously loaded using the <c>LoadSound</c> command.
		/// </para>
		/// <para>
		/// The channel handle returned can subsequently be used to control
		/// the playback of the sound sample specified.
		/// </para>
		/// <para>
		/// The initial volume and pitch of the sound may be modified before
		/// playback using the <c>SoundVolume</c> and <c>SoundPitch</c> commands.
		/// </para>
		/// </summary>
		/// <param name="sound">valid sound handle</param>
		public static int PlaySound(int sound) { return bbPlaySound(sound); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbFreeSound(int sound);
		/// <summary>
		/// <para>
		/// The <c>FreeSound</c> command releases the resources used by
		/// a sound created by a previous call to <c>LoadSound.</c>
		/// </para>
		/// <para>
		/// Usually a program will load all its sound files at startup
		/// and let Blitz3D automatically free the resources when the
		/// program ends.
		/// </para>
		/// <para>
		/// The <c>FreeSound</c> command however provides a way of managing
		/// system resources when large sound files are no longer needed
		/// by a running program.
		/// </para>
		/// </summary>
		/// <param name="sound">valid sound handle</param>
		public static int FreeSound(int sound) { return bbFreeSound(sound); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbLoopSound(int sound);
		/// <summary>
		/// <para>
		/// Enables a sound objects looping property. Subsequent playback
		/// of the sound object using <c>PlaySound</c> will result in continuous
		/// looped playback of the sound.
		/// </para>
		/// </summary>
		/// <param name="sound">valid sound handle</param>
		public static int LoopSound(int sound) { return bbLoopSound(sound); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbSoundPitch(int sound, int samplerate);
		/// <summary>
		/// <para>
		/// Modifies the pitch of an existing sound object by changing its
		/// playback rate.
		/// </para>
		/// <para>
		/// Sounds are commonly recorded at rates such as 22050 and 44100
		/// samples per second and their playback rate defaults to the
		/// recorded rate.
		/// </para>
		/// <para>
		/// Changing the sounds playback rate with the <c>SoundPitch</c> command
		/// will modify the pitch at which it is next played with the
		/// <c>PlaySound</c> command.
		/// </para>
		/// <para>
		/// For more dynamic control see the <c>ChannelPitch</c> command that
		/// allows modifying the pitch of a channel during playback of 
		/// a sound.
		/// </para>
		/// </summary>
		/// <param name="sound">valid sound handle</param>
		/// <param name="samplerate">playback rate in samples per second</param>
		public static int SoundPitch(int sound, int samplerate) { return bbSoundPitch(sound, samplerate); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbSoundVolume(int sound, float volume);
		/// <summary>
		/// <para>
		/// Modifies the default volume of an existing sound object by changing
		/// its amplitude setting.
		/// </para>
		/// <para>
		/// The default volume of a sound returned by <c>LoadSound</c> is 1.0.
		/// </para>
		/// <para>
		/// Use values between 0.0 and 1.0 to cause <c>PlaySound</c> to begin
		/// playback of the specified sound at a quieter volume and values
		/// greater than 1.0 for their volume to be amplified.
		/// </para>
		/// <para>
		/// Use the <c>ChannelVolume</c> command to modify volumes during sound 
		/// playback.
		/// </para>
		/// </summary>
		/// <param name="sound">valid sound handle</param>
		/// <param name="volume">amplitude setting</param>
		public static int SoundVolume(int sound, float volume) { return bbSoundVolume(sound, volume); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbSoundPan(int sound, float pan);
		/// <summary>
		/// <para>
		/// Modifies the default balance of an existing sound object by
		/// changing its pan setting.
		/// </para>
		/// <para>
		/// The <c>pan</c> value can be any float between -1.0 and 1.0 and
		/// modifies the stereo position used the next time the sound 
		/// is played using the <c>PlaySound</c> command.
		/// </para>
		/// <list type="table">
		/// <listheader><term>Pan Value</term><term>Effect</term></listheader>
		/// <item><description>-1</description></item><item><description>sound played through left speaker</description></item>
		/// <item><description>0</description></item><item><description>sound played through both speakers</description></item>
		/// <item><description>1</description></item><item><description>sound played through right speaker</description></item>
		/// </list>
		/// <para>
		/// Use the <c>ChannelPan</c> command to pan the sound during playback.
		/// </para>
		/// </summary>
		/// <param name="sound">valid sound handle</param>
		/// <param name="pan">stereo position</param>
		public static int SoundPan(int sound, float pan) { return bbSoundPan(sound, pan); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbPlayMusic(String filename);
		/// <summary>
		/// <para>
		/// Returns a valid channel handle or 0 if unsuccessful.
		/// </para>
		/// <para>
		/// <c>PlayMusic</c> opens the music file specified and begins
		/// playback.
		/// </para>
		/// <para>
		/// Unlike a combination of <c>LoadSound</c> and <c>PlaySound,</c> <c>PlayMusic</c>
		/// allocates only a small buffer of resources and the music
		/// file is streamed directly from the file.
		/// </para>
		/// <list type="table">
		/// <listheader><term>Format</term><term>FileSize</term><term>Features</term></listheader>
		/// <item><description>raw;wav</description></item><item><description>large</description></item><item><description>industry standard uncompressed</description></item>
		/// <item><description>mod;s3m;xm;it</description></item><item><description>medium</description></item><item><description>8 channel module files</description></item>
		/// <item><description>mid</description></item><item><description>small</description></item><item><description>midi files depend on the system's music synthesizer</description></item>
		/// <item><description>mp3</description></item><item><description>medium</description></item><item><description>requires additional license</description></item>
		/// <item><description>ogg;wma;asf</description></item><item><description>medium</description></item><item><description>compressed and freely distributable</description></item>
		/// </list>
		/// <para>
		/// The channel handle returned can be used to change various
		/// playback settings including volume, pitch as well as 
		/// pause and resume playback itself.
		/// </para>
		/// </summary>
		/// <param name="filename">name of music file</param>
		public static int PlayMusic(String filename) { return bbPlayMusic(filename); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbPlayCDTrack(int track, int mode);
		/// <summary>
		/// <para>
		/// Plays a CD track and returns a sound channel.
		/// </para>
		/// <para>
		/// The behavior of the <c>PlayCDTrack</c> may be modified
		/// with the optional <c>mode</c> parameter:
		/// </para>
		/// <list type="table">
		/// <listheader><term>Mode</term><term>Value</term><term>Description</term></listheader>
		/// <item><description>PLAYCD_SINGLE</description></item><item><description>1</description></item><item><description>play track once - default</description></item>
		/// <item><description>PLAYCD_LOOP</description></item><item><description>2</description></item><item><description>loop track</description></item>
		/// <item><description>PLAYCD_ALL</description></item><item><description>3</description></item><item><description>play track once then continue to next track</description></item>
		/// </list>
		/// <para>
		/// The <c>PlayCDTrack</c> requires the user has a CD playback
		/// facility on their system and that a CD containing
		/// music tracks is currently inserted.
		/// </para>
		/// </summary>
		/// <param name="track">track number to play</param>
		/// <param name="mode">playback mode</param>
		public static int PlayCDTrack(int track, int mode) { return bbPlayCDTrack(track, mode); }
		public static int PlayCDTrack(int track) { return bbPlayCDTrack(track, 1); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbStopChannel(int channel);
		/// <summary>
		/// <para>
		/// Stop any audio being output on a currently playing channel.
		/// </para>
		/// <para>
		/// The <c>PlaySound,</c> <c>PlayMusic</c> and <c>PlayCDTrack</c> functions all return
		/// a channel handle that can be used with <c>StopChannel</c> to cancel the
		/// resulting sound playback.
		/// </para>
		/// </summary>
		/// <param name="channel">valid playback channel</param>
		public static int StopChannel(int channel) { return bbStopChannel(channel); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbPauseChannel(int channel);
		/// <summary>
		/// <para>
		/// Pauses playback in the specified audio channel.
		/// </para>
		/// <para>
		/// Any sound playing from the result of a <c>PlaySound,</c> <c>PlayMusic</c> 
		/// or <c>PlayCDTrack</c> may be paused with the <c>PauseChannel</c> command.
		/// </para>
		/// <para>
		/// Use the <c>ResumeChannel</c> command to continue playback after
		/// pausing an audio channel with <c>PauseChannel.</c>
		/// </para>
		/// </summary>
		/// <param name="channel">valid playback channel</param>
		public static int PauseChannel(int channel) { return bbPauseChannel(channel); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbResumeChannel(int channel);
		/// <summary>
		/// <para>
		/// Continue playback of a previously paused audio channel.
		/// </para>
		/// </summary>
		/// <param name="channel">valid playback channel</param>
		public static int ResumeChannel(int channel) { return bbResumeChannel(channel); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbChannelPitch(int channel, int samplerate);
		/// <summary>
		/// <para>
		/// Modifies the pitch of an active audio channel by changing its
		/// playback rate.
		/// </para>
		/// <para>
		/// Sound sources are commonly recorded at rates such as 22050 
		/// and 44100 samples per second and their playback rate defaults 
		/// to the recorded rate.
		/// </para>
		/// <para>
		/// Changing a channel's playback rate with the <c>ChannelPitch</c> command
		/// will modify the pitch of the recorded audio currently used as
		/// a playback source.
		/// </para>
		/// </summary>
		/// <param name="channel">valid playback channel</param>
		/// <param name="samplerate">playback rate in samples per second</param>
		public static int ChannelPitch(int channel, int samplerate) { return bbChannelPitch(channel, samplerate); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbChannelVolume(int channel, float volume);
		/// <summary>
		/// <para>
		/// Modifies the amplitude of the specified audio channel.
		/// </para>
		/// <para>
		/// A floating point of less than 1.0 will reduce volume
		/// while a value of larger than 1.0 will increase the volume
		/// of the specified channel.
		/// </para>
		/// <para>
		/// Increasing a channel volume above 1.0 should not be 
		/// attempted if distortion and clamping of the audio output 
		/// is to be avoided.
		/// </para>
		/// <para>
		/// To make a channel silent use <c>StopChannel</c> or <c>PauseChannel</c>
		/// as an alternative to a volume setting of 0.0.
		/// </para>
		/// </summary>
		/// <param name="channel">valid playback channel</param>
		/// <param name="volume">volume level</param>
		public static int ChannelVolume(int channel, float volume) { return bbChannelVolume(channel, volume); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbChannelPan(int channel, float pan);
		/// <summary>
		/// <para>
		/// Position the output of an audio channel in left right stereo 
		/// space.
		/// </para>
		/// <list type="table">
		/// <listheader><term>Value</term><term>Effective pan</term></listheader>
		/// <item><description>0.0</description></item><item><description>Left</description></item>
		/// <item><description>0.25</description></item><item><description>Center Left</description></item>
		/// <item><description>0.5</description></item><item><description>Center</description></item>
		/// <item><description>0.75</description></item><item><description>Center Right</description></item>
		/// <item><description>1.0</description></item><item><description>Right</description></item>
		/// </list>
		/// <para>
		/// Panning the position of sound effects in a video game is a
		/// useful technique for adding to the immersive experience.
		/// </para>
		/// </summary>
		/// <param name="channel">valid playback channel</param>
		/// <param name="pan">left right stereo position</param>
		public static int ChannelPan(int channel, float pan) { return bbChannelPan(channel, pan); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbChannelPlaying(int channel);
		/// <summary>
		/// <para>
		/// Returns <c>True</c> if the specified audio output channel is in playback
		/// mode.
		/// </para>
		/// </summary>
		/// <param name="channel">valid playback channel</param>
		public static int ChannelPlaying(int channel) { return bbChannelPlaying(channel); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbOpenMovie(String moviefile);
		/// <summary>
		/// <para>
		/// Locates and starts a movie file playing.
		/// </para>
		/// <para>
		/// Returns a valid movie handle if the function is successful
		/// or 0 if the command fails for any reason.
		/// </para>
		/// <para>
		/// Use the <c>DrawMovie</c> command to see the movie playing.
		/// </para>
		/// <para>
		/// Movie files will typically have the AVI, MPEG and MPG
		/// file extensions.
		/// </para>
		/// <para>
		/// Blitz3D applications may need to specify DirectX8 
		/// requirements or the installation of a particular 
		/// version of Window's media player software if they 
		/// are to support movie files using codecs other than 
		/// MPEG1, CinePak, MotionJPEG and the like.
		/// </para>
		/// </summary>
		/// <param name="moviefile">filename of a movie file</param>
		public static int OpenMovie(String moviefile) { return bbOpenMovie(moviefile); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbCloseMovie(int movie);
		/// <summary>
		/// <para>
		/// Stops and closes an open movie.
		/// </para>
		/// </summary>
		/// <param name="movie">valid open movie file</param>
		public static int CloseMovie(int movie) { return bbCloseMovie(movie); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbDrawMovie(int movie, int x, int y, int width, int height);
		/// <summary>
		/// <para>
		/// Draws the current frame of the specified playing movie
		/// onto the current graphics buffer.
		/// </para>
		/// <para>
		/// The movie must not overlap the edges of the current graphics
		/// buffer or else nothing is drawn. 
		/// </para>
		/// <para>
		/// The Viewport and Origin are not taken into account.
		/// </para>
		/// <para>
		/// See the <c>OpenMovie</c> command for more details regarding supported
		/// movie files and how to open them before using the <c>DrawMovie</c>
		/// command.
		/// </para>
		/// </summary>
		/// <param name="movie">movie handle</param>
		/// <param name="x">horizontal position</param>
		/// <param name="y">vertical position</param>
		/// <param name="width">width of movie in pixels</param>
		/// <param name="height">height of movie in pixels</param>
		public static int DrawMovie(int movie, int x, int y, int width, int height) { return bbDrawMovie(movie, x, y, width, height); }
		public static int DrawMovie(int movie, int x, int y, int width) { return bbDrawMovie(movie, x, y, width, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbMovieWidth(int movie);
		/// <summary>
		/// <para>
		/// Returns the width of a movie.
		/// </para>
		/// </summary>
		/// <param name="movie">movie handle</param>
		public static int MovieWidth(int movie) { return bbMovieWidth(movie); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbMovieHeight(int movie);
		/// <summary>
		/// <para>
		/// Returns the height of a movie.
		/// </para>
		/// </summary>
		/// <param name="movie">movie handle</param>
		public static int MovieHeight(int movie) { return bbMovieHeight(movie); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbMoviePlaying(int movie);
		/// <summary>
		/// <para>
		/// Returns True if the specified movie is playing.
		/// </para>
		/// </summary>
		/// <param name="movie">movie handle</param>
		public static int MoviePlaying(int movie) { return bbMoviePlaying(movie); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbGetKey();
		/// <summary>
		/// <para>
		/// Returns an ascii code corresponding to the key last typed by the
		/// user or 0 if all keyboard events have been reported.
		/// </para>
		/// <para>
		/// As Blitz uses an internal queue for tracking key presses it is
		/// sometimes useful to call <c>GetKey</c> multiple times to empty the
		/// queue. Use the <c>FlushKeys</c> command to completely empty the internal 
		/// key queue.
		/// </para>
		/// <para>
		/// The <c>GetKey</c> function is useful for situations when the user is
		/// expected to type some text.
		/// </para>
		/// <para>
		/// The <c>KeyDown</c> function is more appropriate when the player is 
		/// expected to hold down certain key combinations in a more 
		/// action oriented game environment.
		/// </para>
		/// </summary>
		public static int GetKey() { return bbGetKey(); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbWaitKey();
		/// <summary>
		/// <para>
		/// Waits for a keystroke and returns the ascii code corresponding to 
		/// the key pressed.
		/// </para>
		/// <para>
		/// <c>WaitKey</c> is similar in behavior to the <c>GetKey</c> function but pauses 
		/// program execution until a keystroke is made by the user.
		/// </para>
		/// </summary>
		public static int WaitKey() { return bbWaitKey(); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbKeyDown(int scancode);
		/// <summary>
		/// <para>
		/// Returns <c>True</c> if the specified key on the keyboard is currently
		/// being pressed.
		/// </para>
		/// <para>
		/// Note that keyboard scan codes relate to the physical position of a key
		/// on the keyboard and should not be confused with ascii character codes.
		/// </para>
		/// <para>
		/// There are physical limitations regarding the rows and columns
		/// of some physical keyboards that will not report certain key
		/// combinations. It is advisable to thoroughly test all default
		/// scancode combinations that a game may provide for keyboard 
		/// control.
		/// </para>
		/// </summary>
		/// <param name="scancode">scancode of key to test</param>
		public static int KeyDown(int scancode) { return bbKeyDown(scancode); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbKeyHit(int scancode);
		/// <summary>
		/// <para>
		/// Returns the number of times the specified key has been pressed since
		/// the last time the <c>KeyHit</c> command was called with the specified scancode.
		/// </para>
		/// <para>
		/// <c>KeyHit</c> will only return positive once when a key is pressed where as
		/// <c>KeyDown</c> will repeatedly return True until the specified key is released.
		/// </para>
		/// </summary>
		/// <param name="scancode">scancode of key to test</param>
		public static int KeyHit(int scancode) { return bbKeyHit(scancode); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbFlushKeys();
		/// <summary>
		/// <para>
		/// Resets the state of the internal keyboard map so all keys are considered
		/// up.
		/// </para>
		/// </summary>
		public static int FlushKeys() { return bbFlushKeys(); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbMouseX();
		/// <summary>
		/// <para>
		/// Returns the horizontal display position of the mouse pointer.
		/// </para>
		/// </summary>
		public static int MouseX() { return bbMouseX(); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbMouseY();
		/// <summary>
		/// <para>
		/// Returns the vertical display position of the mouse pointer.
		/// </para>
		/// </summary>
		public static int MouseY() { return bbMouseY(); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbMouseZ();
		/// <summary>
		/// <para>
		/// Returns the mouse wheel position if present.
		/// </para>
		/// <para>
		/// The value returned by <c>MouseZ</c> increases as the user scrolls
		/// the wheel up (away from them) and decreases when the user
		/// scrolls the wheel down (towards them).
		/// </para>
		/// </summary>
		public static int MouseZ() { return bbMouseZ(); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbMouseDown(int button);
		/// <summary>
		/// <para>
		/// The <c>MouseDown</c> function returns <c>True</c> if the specified mouse button
		/// is currently being pressed.
		/// </para>
		/// <para>
		/// Similar to <c>KeyDown</c> a corresponding <c>MouseHit</c> command is available
		/// that will return <c>True</c> only once during the period the specified
		/// button is being pressed.
		/// </para>
		/// </summary>
		/// <param name="button">1,2 or 3 (left, right or middle)</param>
		public static int MouseDown(int button) { return bbMouseDown(button); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbMouseHit(int button);
		/// <summary>
		/// <para>
		/// The <c>MouseHit</c> function returns the number of times the specified 
		/// mouse button has been pressed down since the last call to <c>MouseHit</c>
		/// with the specified button.
		/// </para>
		/// <para>
		/// Use the <c>MouseDown</c> command to test if the specified button is
		/// currently in a depressed state as opposed to if the button has
		/// just been hit. 
		/// </para>
		/// </summary>
		/// <param name="button">1,2 or 3 (left, right or middle)</param>
		public static int MouseHit(int button) { return bbMouseHit(button); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbGetMouse();
		/// <summary>
		/// <para>
		/// Returns the mouse button pressed since the last call to <c>GetMouse</c>
		/// or 0 if none.
		/// </para>
		/// <para>
		/// <c>GetMouse</c> will return 1 if the left button, 2 if the right and
		/// 3 if the middle button has been recently pressed.
		/// </para>
		/// <para>
		/// Mouse button presses are queued internally by Blitz so it is often
		/// useful to call <c>GetMouse</c> repeatedly to empty the queue or use the
		/// <c>FlushMouse</c> command.
		/// </para>
		/// </summary>
		public static int GetMouse() { return bbGetMouse(); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbWaitMouse();
		/// <summary>
		/// <para>
		/// <c>WaitMouse</c> causes the program to halt until a mouse button is pressed
		/// by the user and returns the ID of that button.
		/// </para>
		/// <para>
		/// <c>WaitMouse</c> will wait and return 1 for the left button, 2 for the right 
		/// and 3 for the middle button when pressed.
		/// </para>
		/// </summary>
		public static int WaitMouse() { return bbWaitMouse(); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbShowPointer();
		/// <summary>
		/// <para>
		/// <c>ShowPointer</c> displays the mouse pointer if previously hidden with the
		/// <c>HidePointer</c> command.
		/// </para>
		/// <para>
		/// Has no effect in FullScreen modes.
		/// </para>
		/// </summary>
		public static int ShowPointer() { return bbShowPointer(); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbHidePointer();
		/// <summary>
		/// <para>
		/// <c>HidePointer</c> makes the mouse pointer invisible when moved over
		/// the program window when using <c>Graphics</c> in windowed mode. 
		/// </para>
		/// <para>
		/// The mouse pointer is always hidden in FullScreen <c>Graphics</c> mode.
		/// </para>
		/// </summary>
		public static int HidePointer() { return bbHidePointer(); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbMoveMouse(int x, int y);
		/// <summary>
		/// <para>
		/// The <c>x,</c> <c>y</c> parameters define a location on the graphics display
		/// that the mouse pointer is moved to.
		/// </para>
		/// <para>
		/// By recentering the mouse to the middle of the display every
		/// frame the <c>MouseXSpeed</c> and <c>MouseYSpeed</c> functions can be used
		/// to provide &quot;mouse look&quot; type control common in first person
		/// shooters.
		/// </para>
		/// </summary>
		/// <param name="x">horizontal screen position</param>
		/// <param name="y">vertical screen position</param>
		public static int MoveMouse(int x, int y) { return bbMoveMouse(x, y); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbMouseXSpeed();
		/// <summary>
		/// <para>
		/// Returns the horizontal distance travelled by the mouse since 
		/// the last call to <c>MouseXSpeed</c> or <c>MoveMouse.</c>
		/// </para>
		/// </summary>
		public static int MouseXSpeed() { return bbMouseXSpeed(); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbMouseYSpeed();
		/// <summary>
		/// <para>
		/// Returns the vertical distance travelled by the mouse since the 
		/// last call to <c>MouseYSpeed</c> or <c>MoveMouse.</c>
		/// </para>
		/// </summary>
		public static int MouseYSpeed() { return bbMouseYSpeed(); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbMouseZSpeed();
		/// <summary>
		/// <para>
		/// Returns the number of clicks the mouse scroll wheel has been 
		/// turned since the last call to <c>MouseZSpeed.</c>
		/// </para>
		/// <para>
		/// The result is negative if the wheel is scrolled down (rolled
		/// back) and positive if scrolled up (rolled forward).
		/// </para>
		/// </summary>
		public static int MouseZSpeed() { return bbMouseZSpeed(); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbFlushMouse();
		/// <summary>
		/// <para>
		/// Resets the state of the internal mouse button map so all buttons
		/// are considered up.
		/// </para>
		/// </summary>
		public static int FlushMouse() { return bbFlushMouse(); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbJoyType(int port);
		/// <summary>
		/// <para>
		/// Returns the type of joystick that is currently connected 
		/// to the computer.
		/// </para>
		/// <list type="table">
		/// <listheader><term>JoyType</term><term>Value</term><term>Description</term></listheader>
		/// <item><description>JOYTYPE_NONE</description></item><item><description>0</description></item><item><description>None</description></item>
		/// <item><description>JOYTYPE_DIGITAL</description></item><item><description>1</description></item><item><description>Digital</description></item>
		/// <item><description>JOYTYPE_ANALOG</description></item><item><description>2</description></item><item><description>Analog</description></item>
		/// </list>
		/// <para>
		/// The optional <c>port</c> identifier is required to index
		/// all the joysticks, wheels and other gaming devices
		/// connected to the system.
		/// </para>
		/// </summary>
		/// <param name="port">number of joystick port to check (optional)</param>
		public static int JoyType(int port) { return bbJoyType(port); }
		public static int JoyType() { return bbJoyType(0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbGetJoy(int port);
		/// <summary>
		/// <para>
		/// Returns the number of any button press that
		/// has not already been reported by the <c>GetJoy</c> 
		/// command. 
		/// </para>
		/// <para>
		/// Returns 0 if the system button buffer is
		/// empty.
		/// </para>
		/// <para>
		/// The <c>GetJoy</c> command may be called multiple
		/// times until it signals there are no more
		/// button events queued by returning 0.
		/// </para>
		/// <para>
		/// The optional port identifier provides access to a
		/// particular game controller, joystick or gamepad connected
		/// to the system and positively identified by <c>JoyType.</c>
		/// </para>
		/// </summary>
		/// <param name="port">optional joystick port to read</param>
		public static int GetJoy(int port) { return bbGetJoy(port); }
		public static int GetJoy() { return bbGetJoy(0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbJoyDown(int button, int port);
		/// <summary>
		/// <para>
		/// Returns True if the specified button of the specified
		/// joystick is pressed.
		/// </para>
		/// <para>
		/// The optional port identifier provides access to a
		/// particular game controller, joystick or gamepad connected
		/// to the system and positively identified by <c>JoyType.</c>
		/// </para>
		/// </summary>
		/// <param name="button">number of joystick button to check</param>
		/// <param name="port">number of joystick port to check (optional)</param>
		public static int JoyDown(int button, int port) { return bbJoyDown(button, port); }
		public static int JoyDown(int button) { return bbJoyDown(button, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbJoyHit(int button, int port);
		/// <summary>
		/// <para>
		/// Returns the number of times a specified joystick button 
		/// has been hit since the last time it was specified in 
		/// a <c>JoyHit</c> function call.
		/// </para>
		/// <para>
		/// The optional <c>port</c> identifier provides access to a
		/// particular game controller, joystick or gamepad connected
		/// to the system and positively identified by <c>JoyType.</c>
		/// </para>
		/// </summary>
		/// <param name="button">number of joystick button to check</param>
		/// <param name="port">number of joystick port to check (optional)</param>
		public static int JoyHit(int button, int port) { return bbJoyHit(button, port); }
		public static int JoyHit(int button) { return bbJoyHit(button, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbWaitJoy(int port);
		/// <summary>
		/// <para>
		/// Waits for any joystick button to be pressed and
		/// returns the button identifier.
		/// </para>
		/// <para>
		/// <c>WaitJoy</c> causes the program to pause until any button of the
		/// specified joystick is pressed.
		/// </para>
		/// <para>
		/// If there is no gaming device connected or the optional
		/// port identifier is not a valid device <c>WaitJoy</c>
		/// will not pause but return 0 immediately.
		/// </para>
		/// </summary>
		/// <param name="port">optional joystick port to pause for</param>
		public static int WaitJoy(int port) { return bbWaitJoy(port); }
		public static int WaitJoy() { return bbWaitJoy(0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbFlushJoy();
		/// <summary>
		/// <para>
		/// Resets the state of the internal joystick button map 
		/// so all buttons of all joysticks are considered up
		/// and all joystick events are discarded.
		/// </para>
		/// <para>
		/// <c>FlushJoy</c> is useful when transitioning from control
		/// systems based on the state commands such as <c>JoyDown</c>
		/// to an event style control using the <c>GetJoy</c> command
		/// and any buffered button presses need to be discarded.
		/// </para>
		/// </summary>
		public static int FlushJoy() { return bbFlushJoy(); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbJoyHat(int port);
		/// <summary>
		/// <para>
		/// Returns a compass angle between 0 and 360 degrees in 
		/// which the direction of the D-Pad or &quot;hat&quot; control
		/// is being pressed.
		/// </para>
		/// <para>
		/// <c>JoyHat</c> returns a value of -1 if the &quot;hat&quot; or D-Pad is 
		/// currently centered.
		/// </para>
		/// <para>
		/// The optional port identifier provides access to a
		/// particular game controller, joystick or gamepad connected
		/// to the system and positively identified by <c>JoyType.</c>
		/// </para>
		/// </summary>
		/// <param name="port">number of joystick port to check (optional)</param>
		public static int JoyHat(int port) { return bbJoyHat(port); }
		public static int JoyHat() { return bbJoyHat(0); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbJoyX(int port);
		/// <summary>
		/// <para>
		/// Returns a value between -1.0 and 1.0 representing the direction
		/// of the joystick in the horizontal axis.
		/// </para>
		/// <para>
		/// A value near 0.0 represents the joystick at rest position.
		/// </para>
		/// <para>
		/// Due to the nature of analog joysticks <c>JoyX</c> and the other
		/// axis reading commands are unlikely to ever return an exact 
		/// value of 0.0 and so a tolerance factor may need to be applied
		/// if a rest position is required.
		/// </para>
		/// <para>
		/// The <c>JoyXDir</c> command should be used instead of <c>JoyX</c> 
		/// when only the digital state of the stick is required
		/// (be it left, centered or right).
		/// </para>
		/// <para>
		/// The optional port identifier provides access to a
		/// particular game controller, joystick or gamepad connected
		/// to the system and positively identified by <c>JoyType.</c>
		/// </para>
		/// </summary>
		/// <param name="port">number of joystick port to check (optional)</param>
		public static float JoyX(int port) { return bbJoyX(port); }
		public static float JoyX() { return bbJoyX(0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbJoyXDir(int port);
		/// <summary>
		/// <para>
		/// Returns an integer value of -1, 0 or 1 representing the
		/// horizontal direction of the joystick be it left, centered
		/// or right.
		/// </para>
		/// <list type="table">
		/// <listheader><term>Value</term><term>Direction</term></listheader>
		/// <item><description>-1</description></item><item><description>left</description></item>
		/// <item><description>0</description></item><item><description>centered</description></item>
		/// <item><description>1</description></item><item><description>right</description></item>
		/// </list>
		/// </summary>
		/// <param name="port">number of joystick port to check (optional)</param>
		public static int JoyXDir(int port) { return bbJoyXDir(port); }
		public static int JoyXDir() { return bbJoyXDir(0); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbJoyY(int port);
		/// <summary>
		/// <para>
		/// Returns a value between -1.0 and 1.0 representing the direction
		/// of the the joystick in the vertical axis.
		/// </para>
		/// <para>
		/// See the <c>JoyX</c> command for more details on using joystick 
		/// axis commands.
		/// </para>
		/// </summary>
		/// <param name="port">number of joystick port to check (optional)</param>
		public static float JoyY(int port) { return bbJoyY(port); }
		public static float JoyY() { return bbJoyY(0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbJoyYDir(int port);
		/// <summary>
		/// <para>
		/// Returns an integer value of -1, 0 or 1 representing the
		/// vertical direction of the joystick.
		/// </para>
		/// <list type="table">
		/// <listheader><term>Value</term><term>Direction</term></listheader>
		/// <item><description>-1</description></item><item><description>up</description></item>
		/// <item><description>0</description></item><item><description>centered</description></item>
		/// <item><description>1</description></item><item><description>down</description></item>
		/// </list>
		/// </summary>
		/// <param name="port">number of joystick port to check (optional)</param>
		public static int JoyYDir(int port) { return bbJoyYDir(port); }
		public static int JoyYDir() { return bbJoyYDir(0); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbJoyZ(int port);
		/// <summary>
		/// <para>
		/// Returns a value between -1.0 and 1.0 representing the rotation
		/// axis of the joystick or steering wheel.
		/// </para>
		/// <para>
		/// See the <c>JoyX</c> command for more details on using joystick 
		/// axis commands.
		/// </para>
		/// </summary>
		/// <param name="port">number of joystick port to check (optional)</param>
		public static float JoyZ(int port) { return bbJoyZ(port); }
		public static float JoyZ() { return bbJoyZ(0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbJoyZDir(int port);
		/// <summary>
		/// <para>
		/// Returns an integer value of -1, 0 or 1 representing the
		/// rotation axis of the joystick.
		/// </para>
		/// <list type="table">
		/// <listheader><term>Value</term><term>Direction</term></listheader>
		/// <item><description>-1</description></item><item><description>anti-clockwise</description></item>
		/// <item><description>0</description></item><item><description>centered</description></item>
		/// <item><description>1</description></item><item><description>clockwise</description></item>
		/// </list>
		/// </summary>
		/// <param name="port">number of joystick port to check (optional)</param>
		public static int JoyZDir(int port) { return bbJoyZDir(port); }
		public static int JoyZDir() { return bbJoyZDir(0); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbJoyU(int port);
		/// <summary>
		/// <para>
		/// Returns a value between -1.0 and 1.0 representing the horizontal
		/// direction of the second stick of a dual stick joystick.
		/// </para>
		/// <para>
		/// See the <c>JoyX</c> command for more details on using joystick 
		/// axis commands.
		/// </para>
		/// </summary>
		/// <param name="port">number of joystick port to check (optional)</param>
		public static float JoyU(int port) { return bbJoyU(port); }
		public static float JoyU() { return bbJoyU(0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbJoyUDir(int port);
		/// <summary>
		/// <para>
		/// Returns an integer value of -1, 0 or 1 representing the
		/// horizontal direction of the joystick's second stick be 
		/// it left, centered or right.
		/// </para>
		/// <list type="table">
		/// <listheader><term>Value</term><term>Direction</term></listheader>
		/// <item><description>-1</description></item><item><description>left</description></item>
		/// <item><description>0</description></item><item><description>centered</description></item>
		/// <item><description>1</description></item><item><description>right</description></item>
		/// </list>
		/// </summary>
		/// <param name="port">number of joystick port to check (optional)</param>
		public static int JoyUDir(int port) { return bbJoyUDir(port); }
		public static int JoyUDir() { return bbJoyUDir(0); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbJoyV(int port);
		/// <summary>
		/// <para>
		/// Returns a value between -1.0 and 1.0 representing the vertical
		/// direction of the second stick of a dual stick joystick.
		/// </para>
		/// <para>
		/// See the <c>JoyX</c> command for more details on using joystick 
		/// axis commands.
		/// </para>
		/// </summary>
		/// <param name="port">number of joystick port to check (optional)</param>
		public static float JoyV(int port) { return bbJoyV(port); }
		public static float JoyV() { return bbJoyV(0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbJoyVDir(int port);
		/// <summary>
		/// <para>
		/// Returns an integer value of -1, 0 or 1 representing the
		/// vertical direction of the joystick's second stick.
		/// </para>
		/// <list type="table">
		/// <listheader><term>Value</term><term>Direction</term></listheader>
		/// <item><description>-1</description></item><item><description>up</description></item>
		/// <item><description>0</description></item><item><description>centered</description></item>
		/// <item><description>1</description></item><item><description>down</description></item>
		/// </list>
		/// </summary>
		/// <param name="port">number of joystick port to check (optional)</param>
		public static int JoyVDir(int port) { return bbJoyVDir(port); }
		public static int JoyVDir() { return bbJoyVDir(0); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbJoyYaw(int port);
		/// <summary>
		/// <para>
		/// Returns a value between -1.0 and 1.0 representing the yaw
		/// axis if present of the specified joystick.
		/// </para>
		/// <para>
		/// See the <c>JoyX</c> command for more details on using joystick 
		/// axis commands.
		/// </para>
		/// </summary>
		/// <param name="port">number of joystick port to check (optional)</param>
		public static float JoyYaw(int port) { return bbJoyYaw(port); }
		public static float JoyYaw() { return bbJoyYaw(0); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbJoyPitch(int port);
		/// <summary>
		/// <para>
		/// Returns a value between -1.0 and 1.0 representing the pitch
		/// axis if present of the specified joystick.
		/// </para>
		/// <para>
		/// See the <c>JoyX</c> command for more details on using joystick 
		/// axis commands.
		/// </para>
		/// </summary>
		/// <param name="port">number of joystick port to check (optional)</param>
		public static float JoyPitch(int port) { return bbJoyPitch(port); }
		public static float JoyPitch() { return bbJoyPitch(0); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbJoyRoll(int port);
		/// <summary>
		/// <para>
		/// Returns a value between -1.0 and 1.0 representing the roll
		/// axis if present of the specified joystick.
		/// </para>
		/// <para>
		/// The roll axis of a joystick commonly refers to a joystick's 
		/// twistable stick or rudder feature.
		/// </para>
		/// <para>
		/// See the <c>JoyX</c> command for more details on using joystick 
		/// axis commands.
		/// </para>
		/// </summary>
		/// <param name="port">number of joystick port to check (optional)</param>
		public static float JoyRoll(int port) { return bbJoyRoll(port); }
		public static float JoyRoll() { return bbJoyRoll(0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbGraphics3D(int width, int height, int depth, int mode);
		/// <summary>
		/// <para>
		/// The <c>Graphics3D</c> command resizes the graphics display to the 
		/// specified size in pixels and with the specified display 
		/// properties including color depth and fullscreen options.
		/// </para>
		/// <para>
		/// This command is the same as the <c>Graphics</c> command with the
		/// additional feature that 3D programming is supported following
		/// a succesful call to the <c>Graphics3D</c> command.
		/// </para>
		/// <para>
		/// A simple &quot;Graphics3D 640,480&quot; creates a window on the desktop
		/// ready for 3D program development. Once your program is
		/// running and debug mode has been disabled, the same command 
		/// opens a fullscreen display with standard VGA resolution
		/// of 640 pixels wide by 480 pixels high.
		/// </para>
		/// <para>
		/// The <c>depth</c> parameter is optional, the default value of 0
		/// specifies that Blitz3D select the most appropriate color
		/// depth.
		/// </para>
		/// <para>
		/// The <c>mode</c> parameter may be any of the following values:
		/// </para>
		/// <list type="table">
		/// <listheader><term>Mode</term><term>Value</term><term>Description</term></listheader>
		/// <item><description>GFX_DEFAULT</description></item><item><description>0</description></item><item><description>Default selects FixedWindow in Debug mode or FullScreen in Release</description></item>
		/// <item><description>GFX_FULLSCREEN</description></item><item><description>1</description></item><item><description>FullScreen acheives optimal performance by owning the display</description></item>
		/// <item><description>GFX_WINDOWED</description></item><item><description>2</description></item><item><description>FixedWindow opens a fixed size window placed on the desktop</description></item>
		/// <item><description>GFX_WINDOWEDSCALED</description></item><item><description>3</description></item><item><description>ScaledWindow opens a user sizable window with graphics scaled to fit </description></item>
		/// </list>
		/// <para>
		/// Before using <c>Graphics3D</c> the specified resolution or support
		/// for 3D in Windows should be confirmed with the use of the
		/// <c>GfxMode3DExists</c> or <c>Windowed3D</c> functions respectively.
		/// </para>
		/// </summary>
		/// <param name="width">width of screen resolution</param>
		/// <param name="height">height of screen resolution</param>
		/// <param name="depth">optional color depth for fullscreen modes</param>
		/// <param name="mode">display mode of window created</param>
		public static int Graphics3D(int width, int height, int depth, int mode) { return bbGraphics3D(width, height, depth, mode); }
		public static int Graphics3D(int width, int height, int depth) { return bbGraphics3D(width, height, depth, 0); }
		public static int Graphics3D(int width, int height) { return bbGraphics3D(width, height, 0, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbDither(int enable);
		/// <summary>
		/// <para>
		/// Enables or disables hardware dithering.
		/// </para>
		/// <para>
		/// Only displays configured to use 16 bit color depth benefit from
		/// dithering which attempts to reduce the course shading that lower
		/// color resolutions often suffer.
		/// </para>
		/// <para>
		/// The default is True.
		/// </para>
		/// </summary>
		/// <param name="enable">True to enable dithering</param>
		public static int Dither(int enable) { return bbDither(enable); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbWBuffer(int enable);
		/// <summary>
		/// <para>
		/// Enables or disables w-buffering.
		/// </para>
		/// <para>
		/// Often, 16 bit color depths on common graphics cards will result
		/// in less accurate 16 bit depth buffers being used by the 3D
		/// hardware.
		/// </para>
		/// <para>
		/// W buffering is an alternative to Z buffering that is useful
		/// for increasing depth sorting accuracy with such displays
		/// where the accuracy of sorting distant pixels is reduced
		/// in favour of closer pixels.
		/// </para>
		/// <para>
		/// See the <c>CameraRange</c> command for more information
		/// on contolling depth buffer performance
		/// </para>
		/// <para>
		/// Defaults to True for 16 bit color mode.
		/// </para>
		/// </summary>
		/// <param name="enable"><c>True</c> to enable w-buffered rendering</param>
		public static int WBuffer(int enable) { return bbWBuffer(enable); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbAntiAlias(int enable);
		/// <summary>
		/// <para>
		/// Enables or disables fullscreen antialiasing.
		/// </para>
		/// <para>
		/// Fullscreen antialiasing is a technique used to smooth out the entire screen,
		/// so that jagged lines are made less noticeable.
		/// </para>
		/// <para>
		/// AA rendering options can also be overridden by the user and are often ignored
		/// by the graphics driver.
		/// </para>
		/// <para>
		/// Any AntiAlias setting should be made optional to the user as it may have
		/// a serious impact on the performance of your software on their system.
		/// </para>
		/// <para>
		/// Default is False.
		/// </para>
		/// </summary>
		/// <param name="enable">True to enable fullscreen antialiasing</param>
		public static int AntiAlias(int enable) { return bbAntiAlias(enable); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbWireFrame(int enable);
		/// <summary>
		/// <para>
		/// Enables or disables wireframe rendering.
		/// </para>
		/// <para>
		/// Enabling wire frame rendering will cause <c>RenderWorld</c> to output
		/// only outlines of the polygons that make up the scene.
		/// </para>
		/// <para>
		/// Default is False.
		/// </para>
		/// </summary>
		/// <param name="enable">True to enable wireframe rendering</param>
		public static int WireFrame(int enable) { return bbWireFrame(enable); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbHWMultiTex(int enable);
		/// <summary>
		/// <para>
		/// Enables or disables hardware multitexturing.
		/// </para>
		/// <para>
		/// Hardware multitexturing is the process of rendering 
		/// multiple textures on a single surface using the 
		/// display hardware's multiple pixel pipes if available.
		/// </para>
		/// <para>
		/// Providing the user of your software the option to 
		/// disable hardware multitexturing in favour of the
		/// slower multipass mode may allow them to avoid
		/// certain rendering bugs that exist in older graphics
		/// card drivers.
		/// </para>
		/// <para>
		/// Default is true.
		/// </para>
		/// </summary>
		/// <param name="enable">True to enable hardware multitexturing</param>
		public static int HWMultiTex(int enable) { return bbHWMultiTex(enable); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbRenderWorld(float tween);
		/// <summary>
		/// <para>
		/// All visible entities in the World are rendered on each
		/// enabled camera to the <c>BackBuffer</c> of the current
		/// <c>Graphics3D</c> display device.
		/// </para>
		/// <para>
		/// The area of the <c>Graphics3D</c> display each camera renders
		/// to is specified with the <c>CameraViewport</c> command.
		/// </para>
		/// <para>
		/// A camera will not render if its <c>CameraProjMode</c> has
		/// been set to 0 or it has not been hidden due to a call
		/// to the <c>HideEntity</c> command.
		/// </para>
		/// <para>
		/// A tween value of 0 will render all entities in the same
		/// state they were when <c>CaptureWorld</c> was last called and 
		/// a tween value of 1 (the default) will render all entities in their
		/// current state.
		/// </para>
		/// <para>
		/// The use of tweening allows you to render more than one 
		/// frame per game logic update, while still keeping the 
		/// display smooth. See <c>CaptureWorld</c> for more information
		/// regarding the use of tweening in Blitz3D.
		/// </para>
		/// <para>
		/// Render tweening is an advanced technique, and it is not 
		/// necessary for normal use. See the castle demo included in
		/// the mak (nickname of Mark Sibly, author of Blitz3D) directory 
		/// of the Blitz3D samples section for a demonstration of render 
		/// tweening.
		/// </para>
		/// </summary>
		/// <param name="tween">optional value to render between updates</param>
		public static int RenderWorld(float tween) { return bbRenderWorld(tween); }
		public static int RenderWorld() { return bbRenderWorld(1.0f); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbUpdateWorld(float anim_speed);
		/// <summary>
		/// <para>
		/// Updates all animation in the world and updates all entity positions
		/// based on recent movements and the collision controls in place.
		/// </para>
		/// <para>
		/// The optional <c>anim_speed</c> parameter allows control of the animation
		/// speed of all entities at once. A value of 1 will animate entities
		/// at their usual animation speed, a value of 2 will animate entities
		/// at double their animation speed etc.
		/// </para>
		/// <para>
		/// See the chapter on <c>Collisions</c> for more details on how entity
		/// movement and collisions work in Blitz3D.
		/// </para>
		/// </summary>
		/// <param name="anim_speed">a master control for animation speed. Defaults to 1.</param>
		public static int UpdateWorld(float anim_speed) { return bbUpdateWorld(anim_speed); }
		public static int UpdateWorld() { return bbUpdateWorld(1.0f); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbCaptureWorld();
		/// <summary>
		/// <para>
		/// Creates a snapshot of the world including the position,
		/// rotation, scale and alpha of all entities in the world.
		/// </para>
		/// <para>
		/// The <c>RenderWorld</c> command is capable of rendering frames 
		/// between the world as captured by <c>CaptureWorld</c> and
		/// the world in its current state by using the optional
		/// <c>RenderWorld</c> <c>tween</c> parameter.
		/// </para>
		/// <para>
		/// Often a game is designed to update its controls and
		/// physics at a low frequency such as 10 times per second
		/// in order to reduce network and processor requirements.
		/// </para>
		/// <para>
		/// Calling <c>CaptureWorld</c> after such a game update allows 
		/// the display loop to fill the gaps between game updates
		/// with a smooth transition of frames rendered at various 
		/// periods between the time of the last <c>CaptureWorld</c> and 
		/// the time of the most recent <c>UpdateWorld.</c>
		/// </para>
		/// <para>
		/// The position of individual vertices are not captured
		/// by the CaptureWorld command and so VertexCoords
		/// based animation must be tweened manually.
		/// </para>
		/// </summary>
		public static int CaptureWorld() { return bbCaptureWorld(); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbClearWorld(int entities, int brushes, int textures);
		/// <summary>
		/// <para>
		/// Clears all entities, brushes and / or textures from system memory.
		/// </para>
		/// <para>
		/// As soon as a resource is freed due to a call to <c>ClearWorld</c> its 
		/// handle must never be used again. 
		/// </para>
		/// <para>
		/// Trying to do so will cause the fatal &quot;Entity Does Not Exist&quot;
		/// or similar runtime error.
		/// </para>
		/// <para>
		/// This command is useful for when a game level has finished and you 
		/// wish to load a different level with new entities, brushes and 
		/// textures. 
		/// </para>
		/// </summary>
		/// <param name="entities"><c>True</c> to free all entities, <c>False</c> not to. Defaults to true.</param>
		/// <param name="brushes"><c>True</c> to free all brushes, <c>False</c> not to. Defaults to true.</param>
		/// <param name="textures"><c>True</c> to free all textures, <c>False</c> not to. Defaults to true.</param>
		public static int ClearWorld(int entities, int brushes, int textures) { return bbClearWorld(entities, brushes, textures); }
		public static int ClearWorld(int entities, int brushes) { return bbClearWorld(entities, brushes, 1); }
		public static int ClearWorld(int entities) { return bbClearWorld(entities, 1, 1); }
		public static int ClearWorld() { return bbClearWorld(1, 1, 1); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbTrisRendered();
		/// <summary>
		/// <para>
		/// Returns the number of triangles rendered during the most recent 
		/// <c>RenderWorld.</c>
		/// </para>
		/// <para>
		/// The triangle count of a scene is an important resource and errors 
		/// in such things as a 3D file's level of detail can slow down
		/// even the fastest computer. 
		/// </para>
		/// <para>
		/// Always make sure the models you are loading are built for 
		/// game use and not movie production, the later a likely cause 
		/// of poorly performing programs.
		/// </para>
		/// </summary>
		public static int TrisRendered() { return bbTrisRendered(); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbCreateCamera(int parent);
		/// <summary>
		/// <para>
		/// Returns the handle of a newly created camera entity.
		/// </para>
		/// <para>
		/// <c>RenderWorld</c> uses the camera entities in the world
		/// to render to the display. At least one camera is
		/// required for <c>RenderWorld</c> to draw anything to the 
		/// display.
		/// </para>
		/// <para>
		/// <c>CameraViewport</c> may be used to modify the region of
		/// the display onto which the camera renders. The default
		/// viewport of a new camera is a region that covers
		/// the entire display.
		/// </para>
		/// <para>
		/// Multiple cameras may be used for split screen,
		/// overlay and picture in picture style effects.
		/// </para>
		/// <para>
		/// The <c>EntityOrder</c> command can be used to control 
		/// the order in which multiple cameras are rendered,
		/// see also CameraClsMode and CameraViewport which
		/// are 2 other commands useful when setting up a 
		/// multi-camera enviroment.
		/// </para>
		/// <para>
		/// The optional <c>parent</c> parameter attaches the new camera
		/// to a parent entity. See the <c>EntityParent</c> command for 
		/// more details on the effects of entity parenting. 
		/// </para>
		/// </summary>
		/// <param name="parent">parent entity of camera</param>
		public static int CreateCamera(int parent) { return bbCreateCamera(parent); }
		public static int CreateCamera() { return bbCreateCamera(0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbCameraViewport(int camera, int x, int y, int width, int height);
		/// <summary>
		/// <para>
		/// Sets the camera viewport position and size.
		/// </para>
		/// <para>
		/// The camera viewport is the area of the 2D screen that the
		/// 3D graphics as viewed by the camera are displayed in. 
		/// </para>
		/// <para>
		/// Setting the camera viewport allows you to achieve 
		/// split-screen, overlays and rear-view mirror effects.
		/// </para>
		/// <para>
		/// The world is rendered in a viewport such that the
		/// camera's horizontal scale is preserved and the vertical 
		/// preserves the aspect ratio.
		/// </para>
		/// <para>
		/// In situations in which one camera is overlaid ontop
		/// another such as a game's scanner or user interface, the
		/// <c>EntityOrder</c> command can be used to modify the
		/// order in which viewports are rendered.
		/// </para>
		/// </summary>
		/// <param name="camera">camera handle</param>
		/// <param name="x">x coordinate of top left hand corner of viewport</param>
		/// <param name="y">y coordinate of top left hand corner of viewport</param>
		/// <param name="width">width of viewport</param>
		/// <param name="height">height of viewport</param>
		public static int CameraViewport(int camera, int x, int y, int width, int height) { return bbCameraViewport(camera, x, y, width, height); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbCameraClsMode(int camera, int cls_color, int cls_zbuffer);
		/// <summary>
		/// <para>
		/// Sets camera clear mode.
		/// </para>
		/// <para>
		/// By default each camera contributing a view to the
		/// <c>RenderWorld</c> command will clear both the color and
		/// depth buffers before rendering every entity in view.
		/// A False argument for either the cls_color or 
		/// cls_zbuffer parameters modifies the specified
		/// camera's behavior in this respect.
		/// </para>
		/// <para>
		/// Overlay cameras often disable the automatic clearing
		/// of the color buffer so that the scene rendered already
		/// by the main camera appears behind the overlay viewport.
		/// </para>
		/// <para>
		/// The advanced technique of multiple pass rendering
		/// where layers such as shadows and haze are rendered
		/// using multiple calls to <c>RenderWorld</c> before a single
		/// <c>Flip</c> often require cameras where both color
		/// and depth buffer clearing is disabled.
		/// </para>
		/// </summary>
		/// <param name="camera">camera handle</param>
		/// <param name="cls_color"><c>False</c> to disable clearing of color buffer at rendertime</param>
		/// <param name="cls_zbuffer"><c>False</c> to disable clearing of depth buffer at rendertime</param>
		public static int CameraClsMode(int camera, int cls_color, int cls_zbuffer) { return bbCameraClsMode(camera, cls_color, cls_zbuffer); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbCameraClsColor(int camera, float red, float green, float blue);
		/// <summary>
		/// <para>
		/// Sets camera background color.
		/// </para>
		/// <para>
		/// Defaults to 0,0,0 (black).
		/// </para>
		/// <para>
		/// See the <c>Color</c> command for more information on combining
		/// red, green and blue values to define colors.
		/// </para>
		/// </summary>
		/// <param name="camera">camera handle</param>
		/// <param name="red">red value of camera background color</param>
		/// <param name="green">green value of camera background color</param>
		/// <param name="blue">blue value of camera background color</param>
		public static int CameraClsColor(int camera, float red, float green, float blue) { return bbCameraClsColor(camera, red, green, blue); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbCameraRange(int camera, float near, float far);
		/// <summary>
		/// <para>
		/// Sets camera range.
		/// </para>
		/// <para>
		/// Defaults to 1,1000.
		/// </para>
		/// <para>
		/// The distance parameters used in <c>CameraRange</c> define two planes.
		/// </para>
		/// <para>
		/// The near plane will clip any objects that come too close to the
		/// camera while the far plane will ensure the camera does not render
		/// objects that are too far away (a common cause of slowdown
		/// in games).
		/// </para>
		/// <para>
		/// Fog can be used to soften the transition when objects approach
		/// a <c>CameraRange's</c> <c>far</c> distance, see the <c>CameraFogRange</c> command
		/// for more details.
		/// </para>
		/// <para>
		/// The distance between <c>near</c> and <c>far</c> also affects the precision
		/// of the depth buffer which is used to determine which pixels from
		/// which polygon are drawn when they overlap or even intersect.
		/// </para>
		/// <para>
		/// See the <c>WBuffer</c> command for another command that can
		/// affect depth buffer performance.
		/// </para>
		/// </summary>
		/// <param name="camera">camera handle</param>
		/// <param name="near">distance in front of camera that 3D objects start being drawn</param>
		/// <param name="far">distance in front of camera that 3D object stop being drawn</param>
		public static int CameraRange(int camera, float near, float far) { return bbCameraRange(camera, near, far); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbCameraZoom(int camera, float zoom);
		/// <summary>
		/// <para>
		/// Sets zoom factor for a camera. Defaults to 1.0.
		/// </para>
		/// <para>
		/// Values between 0.01 and 1.0 causes objects to look 
		/// smaller. Zoom values larger than 1.0 cause objects 
		/// to appear larger.
		/// </para>
		/// </summary>
		/// <param name="camera">camera handle</param>
		/// <param name="zoom">zoom factor of camera</param>
		public static int CameraZoom(int camera, float zoom) { return bbCameraZoom(camera, zoom); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbCameraProjMode(int camera, int mode);
		/// <summary>
		/// <para>
		/// Sets the camera projection mode.
		/// </para>
		/// <list type="table">
		/// <listheader><term>Mode</term><term>Value</term><term>Description</term></listheader>
		/// <item><description>PROJ_NONE</description></item><item><description>0</description></item><item><description>No projection - disables camera (faster than HideEntity)</description></item>
		/// <item><description>PROJ_PERSPECTIVE</description></item><item><description>1</description></item><item><description>Perspective projection (default)</description></item>
		/// <item><description>PROJ_ORTHO</description></item><item><description>2</description></item><item><description>Orthographic projection</description></item>
		/// </list>
		/// <para>
		/// Standard perspective projection uses a zoom variable to make
		/// objects further away from the camera appear smaller.
		/// </para>
		/// <para>
		/// In contrast, orthographic projection involves a camera with
		/// infinite zoom where the disatance from camera does not affect
		/// the size an object is viewed. Orthographic projection is 
		/// also known as isometric projection.
		/// </para>
		/// <para>
		/// The <c>CameraZoom</c> of an orthorgaphic camera instead affects
		/// the scale of graphics rendered with orthographic projection.
		/// </para>
		/// <para>
		/// Unfortunately Blitz3D <c>Terrains</c> are not compatible with
		/// orthographic projection due to the real time level of 
		/// detail algorithm used.
		/// </para>
		/// </summary>
		/// <param name="camera">camera handle</param>
		/// <param name="mode">projection mode</param>
		public static int CameraProjMode(int camera, int mode) { return bbCameraProjMode(camera, mode); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbCameraFogMode(int camera, int mode);
		/// <summary>
		/// <para>
		/// Sets the camera fog mode.
		/// </para>
		/// <list type="table">
		/// <listheader><term>Mode</term><term>Description</term></listheader>
		/// <item><description>0</description></item><item><description>No fog (default)</description></item>
		/// <item><description>1</description></item><item><description>Linear fog</description></item>
		/// </list>
		/// <para>
		/// This will enable/disable fogging, a technique used to gradually fade
		/// out graphics the further they are away from the camera. This can be used
		/// to avoid 'pop-up', the moment at which 3D objects suddenly appear on the
		/// horizon which itself is controlled by the <c>far</c> parameter of the
		/// <c>CameraRange</c> command.
		/// </para>
		/// <para>
		/// The default fog color is black and the default fog range
		/// is 1-1000. Change these values with the <c>CameraFogColor</c>
		/// and <c>CameraFogRange</c> commands respectively. 
		/// </para>
		/// <para>
		/// Each camera can have its own fog mode, for multiple on-screen
		/// fog effects.
		/// </para>
		/// </summary>
		/// <param name="camera">camera handle</param>
		/// <param name="mode">camera mode</param>
		public static int CameraFogMode(int camera, int mode) { return bbCameraFogMode(camera, mode); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbCameraFogColor(int camera, float red, float green, float blue);
		/// <summary>
		/// <para>
		/// Sets camera fog color.
		/// </para>
		/// <para>
		/// See the <c>Color</c> command for more information about
		/// combining red, green and blue values to define
		/// colors in Blitz3D.
		/// </para>
		/// </summary>
		/// <param name="camera">camera handle</param>
		/// <param name="red">red value of value</param>
		/// <param name="green">green value of fog</param>
		/// <param name="blue">blue value of fog</param>
		public static int CameraFogColor(int camera, float red, float green, float blue) { return bbCameraFogColor(camera, red, green, blue); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbCameraFogRange(int camera, float near, float far);
		/// <summary>
		/// <para>
		/// Sets camera fog range.
		/// </para>
		/// <para>
		/// The <c>near</c> parameter specifies at what distance in front of the 
		/// camera specified that the fogging effect will start.
		/// </para>
		/// <para>
		/// The <c>far</c> parameter specifies at what distance in front of the 
		/// camera that the fogging effect will end. All pixels rendered
		/// beyond this point will be completely faded to the fog color.
		/// </para>
		/// </summary>
		/// <param name="camera">camera handle</param>
		/// <param name="near">distance in front of camera that fog starts</param>
		/// <param name="far">distance in front of camera that fog ends</param>
		public static int CameraFogRange(int camera, float near, float far) { return bbCameraFogRange(camera, near, far); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbCameraProject(int camera, float x, float y, float z);
		/// <summary>
		/// <para>
		/// Projects the world coordinate x,y,z on to the 2D screen.
		/// </para>
		/// <para>
		/// Use the <c>ProjectedX,</c> <c>ProjectedY</c> and <c>ProjectedZ</c> functions
		/// to determine the pixel location on the camera's viewport and its
		/// distance from the camera the global position specified would
		/// be rendered at.
		/// </para>
		/// <para>
		/// <c>CameraProject</c> is useful for positioning 2D lines or text
		/// relative to the world positions of corresponding entity
		/// or verticy locations. 
		/// </para>
		/// </summary>
		/// <param name="camera">camera handle</param>
		/// <param name="x">world coordinate x</param>
		/// <param name="y">world coordinate y</param>
		/// <param name="z">world coordinate z</param>
		public static int CameraProject(int camera, float x, float y, float z) { return bbCameraProject(camera, x, y, z); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbProjectedX();
		/// <summary>
		/// <para>
		/// Returns the viewport x coordinate of the most recently 
		/// executed <c>CameraProject</c> command.
		/// </para>
		/// </summary>
		public static float ProjectedX() { return bbProjectedX(); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbProjectedY();
		/// <summary>
		/// <para>
		/// Returns the viewport y coordinate of the most recently
		/// executed <c>CameraProject</c> command.
		/// </para>
		/// </summary>
		public static float ProjectedY() { return bbProjectedY(); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbProjectedZ();
		/// <summary>
		/// <para>
		/// Returns the viewport z coordinate of the most recently
		/// executed CameraProject command. This value corresponds 
		/// to the distance into the screen the point is located after 
		/// a <c>CameraProject</c> transforms a global point into the
		/// camera's viewport space.
		/// </para>
		/// </summary>
		public static float ProjectedZ() { return bbProjectedZ(); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbEntityInView(int entity, int camera);
		/// <summary>
		/// <para>
		/// Returns true if the specified entity is visible to the specified camera.
		/// </para>
		/// <para>
		/// If the entity is a mesh, its bounding box will be checked for visibility. 
		/// </para>
		/// <para>
		/// For all other types of entities, only their centre position will be checked.
		/// </para>
		/// <para>
		/// For animated meshes it is important their bounding box allow for all
		/// possible frames of animation for EntityInView to function correctly.
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		/// <param name="camera">camera handle</param>
		public static int EntityInView(int entity, int camera) { return bbEntityInView(entity, camera); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbAmbientLight(float red, float green, float blue);
		/// <summary>
		/// <para>
		/// Sets the ambient light color.
		/// </para>
		/// <para>
		/// Ambient light is added to all surfaces during a <c>RenderWorld.</c>
		/// </para>
		/// <para>
		/// Ambient light has no position or direction and hence does not
		/// contribute to the shading of surfaces just their overall brightness.
		/// </para>
		/// <para>
		/// See <c>CreateLight</c> for how to add lights that provide 3D shading.
		/// </para>
		/// <para>
		/// The red, green and blue values should be in the range 0..255.
		/// </para>
		/// <para>
		/// See the <c>Color</c> command for more information about combining
		/// red, green and blue values to describe specific colors.
		/// </para>
		/// <para>
		/// The default ambient light color is 127,127,127.
		/// </para>
		/// </summary>
		/// <param name="red">red ambient light value</param>
		/// <param name="green">green ambient light value</param>
		/// <param name="blue">blue ambient light value</param>
		public static int AmbientLight(float red, float green, float blue) { return bbAmbientLight(red, green, blue); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbCreateLight(int light_type, int parent);
		/// <summary>
		/// <para>
		/// Creates a light emitting entity.
		/// </para>
		/// <para>
		/// By creating a light with the <c>CreateLight</c> function
		/// surfaces in range of the light have additional
		/// light added to their color based on the angle the
		/// surface is to the light and the diffuse and specular
		/// properties of the surface.
		/// </para>
		/// <para>
		/// The optional <c>light_type</c> parameter allows you to specify
		/// from one of the following light types:
		/// </para>
		/// <list type="table">
		/// <listheader><term>Name</term><term>Value</term><term>Description</term></listheader>
		/// <item><description>LIGHT_DIRECTIONAL</description></item><item><description>1</description></item><item><description>Specific directional with infinite range and no position.</description></item>
		/// <item><description>LIGHT_POINT</description></item><item><description>2</description></item><item><description>Specific range and position.</description></item>
		/// <item><description>LIGHT_SPOT</description></item><item><description>3</description></item><item><description>Specific range position and angle.</description></item>
		/// </list>
		/// <para>
		/// Point lights radiate light evenly from a single point
		/// while spot lights create a cone of light emitting
		/// from a single point aligned to the light entitys' 
		/// current orientation.
		/// </para>
		/// <para>
		/// A directional light is useful for emulating light
		/// sources like the sun where it is so distant and has
		/// such a large range it is simpler to reference only 
		/// the angle of its shine.
		/// </para>
		/// <para>
		/// The optional <c>parent</c> parameter attaches the new light
		/// to a parent entity. See the <c>EntityParent</c> command for 
		/// more details on the effects of entity parenting. 
		/// </para>
		/// <para>
		/// Typically an outdoor daytime scene will require a single 
		/// directional light entity set to an appropriate angle and 
		/// brightness, see <c>RotateEntity</c> and <c>CreateLight</c> for more
		/// information. Adjustments to <c>AmbientLight</c> are helpful 
		/// for replicating the effect the entire sky is having on 
		/// the amount of light in the world.
		/// </para>
		/// <para>
		/// For nighttime and indoor scenes, a combination of point
		/// and spot light entities can be controlled in each room
		/// for dramatic shading and mood. 
		/// </para>
		/// <para>
		/// See <c>BrushShininess</c> and <c>EntityShininess</c> for more 
		/// information about the use of specular lighting.
		/// </para>
		/// <para>
		/// Due to hardware limitations no single location
		/// in the world should be in the range and hence
		/// affected by more than a total of 8 lights.
		/// </para>
		/// <para>
		/// Unlike point and spot lights, directional lights 
		/// have infinite range and so their position is ignored
		/// and are always included in the lighting 
		/// calculations during <c>RenderWorld.</c>
		/// </para>
		/// </summary>
		/// <param name="light_type">type of light</param>
		/// <param name="parent">parent entity of light</param>
		public static int CreateLight(int light_type, int parent) { return bbCreateLight(light_type, parent); }
		public static int CreateLight(int light_type) { return bbCreateLight(light_type, 0); }
		public static int CreateLight() { return bbCreateLight(0, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbLightRange(int light, float range);
		/// <summary>
		/// <para>
		/// Sets a light's maximum effective distance.
		/// </para>
		/// <para>
		/// The default range of a light is 1000.0.
		/// </para>
		/// </summary>
		/// <param name="light">point or spot light entity</param>
		/// <param name="range">range of light</param>
		public static int LightRange(int light, float range) { return bbLightRange(light, range); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbLightColor(int light, float red, float green, float blue);
		/// <summary>
		/// <para>
		/// Sets the color and brightness of a light.
		/// </para>
		/// <para>
		/// See the <c>Color</c> command for more information on combining
		/// red, green and blue values to define colors.
		/// </para>
		/// <para>
		/// Values of 255,255,255 sets a light to emit bright white light.
		/// </para>
		/// <para>
		/// A value of black or 0,0,0 effectively disables a light.
		/// </para>
		/// <para>
		/// Values of less than 0 can be used to remove light from a
		/// scene. This is known as 'negative lighting', and is useful 
		/// for shadow effects.
		/// </para>
		/// </summary>
		/// <param name="light">light handle</param>
		/// <param name="red">red value of light</param>
		/// <param name="green">green value of light</param>
		/// <param name="blue">blue value of light</param>
		public static int LightColor(int light, float red, float green, float blue) { return bbLightColor(light, red, green, blue); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbLightConeAngles(int light, float inner_angle, float outer_angle);
		/// <summary>
		/// <para>
		/// Sets the 'cone' angle for a SpotLight.
		/// </para>
		/// <para>
		/// The default light cone angles setting is 0,90.
		/// </para>
		/// </summary>
		/// <param name="light">light handle</param>
		/// <param name="inner_angle">inner angle of cone in degrees</param>
		/// <param name="outer_angle">outer angle of cone in degrees</param>
		public static int LightConeAngles(int light, float inner_angle, float outer_angle) { return bbLightConeAngles(light, inner_angle, outer_angle); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbCreatePivot(int parent);
		/// <summary>
		/// <para>
		/// Creates a pivot entity.
		/// </para>
		/// <para>
		/// Pivots have position, scale and orientation but no geometry 
		/// and so are always invisible themselves.
		/// </para>
		/// <para>
		/// Pivots make useful parent entities where they can be used
		/// to control the visibility, position and orientation of 
		/// their children.
		/// </para>
		/// <para>
		/// Pivots are also used for the bones when loading animated
		/// b3d files with the <c>LoadAnimMesh</c> command.
		/// </para>
		/// <para>
		/// The optional <c>parent</c> parameter attaches the new pivot
		/// to a parent entity. See the <c>EntityParent</c> command for 
		/// more details on the effects of entity parenting. 
		/// </para>
		/// </summary>
		/// <param name="parent">optional parent entity of new pivot</param>
		public static int CreatePivot(int parent) { return bbCreatePivot(parent); }
		public static int CreatePivot() { return bbCreatePivot(0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbCreateCube(int parent);
		/// <summary>
		/// <para>
		/// Creates a geometric cube, a mesh the shape of a 
		/// square box.
		/// </para>
		/// <para>
		/// The new cube extends from -1,-1,-1 to +1,+1,+1.
		/// </para>
		/// <para>
		/// Creation of cubes, cylinders and cones are great 
		/// for placeholders during initial program development
		/// when a game's art resources may only be in their
		/// planning stages.
		/// </para>
		/// <para>
		/// The optional <c>parent</c> parameter attaches the new cube
		/// to a parent entity. See the <c>EntityParent</c> command for 
		/// more details on the effects of entity parenting. 
		/// </para>
		/// <para>
		/// Parenting a semitransparent cube to an object to
		/// visually represent its <c>EntityBox</c> collision settings
		/// is often useful when fine tuning object collisions.
		/// </para>
		/// </summary>
		/// <param name="parent">optional parent entity of a new cube</param>
		public static int CreateCube(int parent) { return bbCreateCube(parent); }
		public static int CreateCube() { return bbCreateCube(0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbCreateSphere(int segments, int parent);
		/// <summary>
		/// <para>
		/// Creates a geometric sphere, a mesh the shape of a round ball.
		/// </para>
		/// <para>
		/// The sphere will be centred at 0,0,0 and will have a radius of 1. 
		/// </para>
		/// <para>
		/// The optional segements value affects how many triangles are used
		/// in the resulting mesh:
		/// </para>
		/// <list type="table">
		/// <listheader><term>Value</term><term>Triangles</term><term>Comment</term></listheader>
		/// <item><description>8</description></item><item><description>224</description></item><item><description>Bare minimum amount of polygons for a sphere</description></item>
		/// <item><description>16</description></item><item><description>960</description></item><item><description>Smooth looking sphere at medium-far distances</description></item>
		/// <item><description>32</description></item><item><description>3968</description></item><item><description>Smooth sphere at close distances</description></item>
		/// </list>
		/// <para>
		/// The optional <c>parent</c> parameter attaches the new sphere
		/// to a parent entity. See the <c>EntityParent</c> command for 
		/// more details on the effects of entity parenting. 
		/// </para>
		/// </summary>
		/// <param name="segments">sphere detail. Defaults to 8.</param>
		/// <param name="parent">parent entity of new sphere</param>
		public static int CreateSphere(int segments, int parent) { return bbCreateSphere(segments, parent); }
		public static int CreateSphere(int segments) { return bbCreateSphere(segments, 0); }
		public static int CreateSphere() { return bbCreateSphere(8, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbCreateCylinder(int segments, int solid, int parent);
		/// <summary>
		/// <para>
		/// Creates a mesh entity the shape of a cylinder with optional ends.
		/// </para>
		/// <para>
		/// The cylinder will be centred at 0,0,0 and will have a radius of 1. 
		/// </para>
		/// <para>
		/// The segments value must be in the range 3-100 inclusive and results
		/// in cylinders with the following triangle counts:
		/// </para>
		/// <list type="table">
		/// <listheader><term>Value</term><term>Triangles</term><term>Comment</term></listheader>
		/// <item><description>3</description></item><item><description>8</description></item><item><description>A prism</description></item>
		/// <item><description>8</description></item><item><description>28</description></item><item><description>Bare minimum amount of polygons for a cylinder</description></item>
		/// <item><description>16</description></item><item><description>60</description></item><item><description>Smooth cylinder at medium-far distances</description></item>
		/// <item><description>32</description></item><item><description>124</description></item><item><description>Smooth cylinder at close distances </description></item>
		/// </list>
		/// <para>
		/// The optional <c>parent</c> parameter attaches the new cylinder
		/// to a parent entity. See the <c>EntityParent</c> command for 
		/// more details on the effects of entity parenting. 
		/// </para>
		/// </summary>
		/// <param name="segments">cylinder detail. Defaults to 8.</param>
		/// <param name="solid"><c>True</c> for a cylinder <c>False</c> for a tube. Defaults to <c>True.</c></param>
		/// <param name="parent">parent entity of cylinder</param>
		public static int CreateCylinder(int segments, int solid, int parent) { return bbCreateCylinder(segments, solid, parent); }
		public static int CreateCylinder(int segments, int solid) { return bbCreateCylinder(segments, solid, 0); }
		public static int CreateCylinder(int segments) { return bbCreateCylinder(segments, 1, 0); }
		public static int CreateCylinder() { return bbCreateCylinder(8, 1, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbCreateCone(int segments, int solid, int parent);
		/// <summary>
		/// <para>
		/// Creates a mesh entity the shape of a cone with optional base.
		/// </para>
		/// <para>
		/// The cone will be centred at 0,0,0 and the base of the cone will have a 
		/// radius of 1. 
		/// </para>
		/// <para>
		/// The segments value has a range 3-100 inclusive and results
		/// in cones with the following triangle counts:
		/// </para>
		/// <list type="table">
		/// <listheader><term>Value</term><term>Triangles</term><term>Description</term></listheader>
		/// <item><description>4</description></item><item><description>6</description></item><item><description>A pyramid</description></item>
		/// <item><description>8</description></item><item><description>14</description></item><item><description>Bare minimum amount of polygons for a cone</description></item>
		/// <item><description>16</description></item><item><description>30</description></item><item><description>Smooth cone at medium-far distances</description></item>
		/// <item><description>32</description></item><item><description>62</description></item><item><description>Smooth cone at close distances </description></item>
		/// </list>
		/// <para>
		/// The optional <c>parent</c> parameter attaches the new cone
		/// to a parent entity. See the <c>EntityParent</c> command for 
		/// more details on the effects of entity parenting. 
		/// </para>
		/// </summary>
		/// <param name="segments">cone detail. Defaults to 8.</param>
		/// <param name="solid"><c>True</c> for a cone with a base <c>False</c> for a cone without a base. Defaults to <c>True.</c></param>
		/// <param name="parent">parent entity of cone</param>
		public static int CreateCone(int segments, int solid, int parent) { return bbCreateCone(segments, solid, parent); }
		public static int CreateCone(int segments, int solid) { return bbCreateCone(segments, solid, 0); }
		public static int CreateCone(int segments) { return bbCreateCone(segments, 1, 0); }
		public static int CreateCone() { return bbCreateCone(8, 1, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbCreatePlane(int divisions, int parent);
		/// <summary>
		/// <para>
		/// Creates a geometric plane, a flat surface with zero height that extends 
		/// infinitely in the x and z axis.
		/// </para>
		/// <para>
		/// The optional <c>divisions</c> parameter determines how many sub divisions of 
		/// polygons the viewable area of the plane will be rendered with which
		/// is important when their are point and spot lights contributing to the
		/// lighting of a plane's surface.
		/// </para>
		/// <para>
		/// Due to its inifinite nature a plane is not a mesh based entity
		/// so unlike <c>CreateCube,</c> <c>CreateSphere,</c> <c>CreateCylinder</c> and
		/// <c>CreateCone,</c> mesh based commands must not be used on planes.
		/// </para>
		/// <para>
		/// The optional <c>parent</c> parameter attaches the new plane
		/// to a parent entity. See the <c>EntityParent</c> command for 
		/// more details on the effects of entity parenting. 
		/// </para>
		/// </summary>
		/// <param name="divisions">sub divisions of plane in the range 1-16. The default value is 1.</param>
		/// <param name="parent">parent entity of plane</param>
		public static int CreatePlane(int divisions, int parent) { return bbCreatePlane(divisions, parent); }
		public static int CreatePlane(int divisions) { return bbCreatePlane(divisions, 0); }
		public static int CreatePlane() { return bbCreatePlane(1, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbCreateMirror(int parent);
		/// <summary>
		/// <para>
		/// Creates a mirror entity which is an invisible plane with a surface that 
		/// reflects all visible geometry when rendered.
		/// </para>
		/// <para>
		/// The optional <c>parent</c> parameter attaches the new mirror
		/// to a parent entity. See the <c>EntityParent</c> command for 
		/// more details on the effects of entity parenting. 
		/// </para>
		/// <para>
		/// See <c>CreatePlane</c> for more information about the size and shape of a
		/// mirror's geometry.
		/// </para>
		/// </summary>
		/// <param name="parent">parent entity of mirror</param>
		public static int CreateMirror(int parent) { return bbCreateMirror(parent); }
		public static int CreateMirror() { return bbCreateMirror(0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbCopyEntity(int entity, int parent);
		/// <summary>
		/// <para>
		/// Returns a duplicate or clone of the specified entity.
		/// </para>
		/// <para>
		/// Surfaces of mesh based entities are not duplicated but
		/// shared with the clone returned by <c>CopyEntity.</c> Use 
		/// the <c>CopyMesh</c> command to duplicate a mesh
		/// entity with unique surfaces.
		/// </para>
		/// <para>
		/// The optional <c>parent</c> parameter attaches the new clone
		/// to a parent entity. See the <c>EntityParent</c> command for 
		/// more details on the effects of entity parenting. 
		/// </para>
		/// <para>
		/// CopyEntity is fater than repeatedly calling LoadEntity
		/// and will save on memory.
		/// </para>
		/// </summary>
		/// <param name="entity">Entity to duplicate</param>
		/// <param name="parent">Entity that will act as parent to the copy.</param>
		public static int CopyEntity(int entity, int parent) { return bbCopyEntity(entity, parent); }
		public static int CopyEntity(int entity) { return bbCopyEntity(entity, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbLoadTexture(String file, int flags);
		/// <summary>
		/// <para>
		/// Load a texture from an image file and returns the texture's handle.
		/// </para>
		/// <para>
		/// Supported file formats include BMP, PNG, TGA, JPG and DDS.
		/// </para>
		/// <para>
		/// Only PNG, TGA and specific DDS formats support an alpha channel 
		/// which provides per pixel transparency information.
		/// </para>
		/// <para>
		/// See <c>CreateTexture</c> for more detailed descriptions of the texture flags and
		/// the <c>TextureFilter</c> command for an alternative method of setting the texture
		/// flags of a loaded texture based on the texture file's name.
		/// </para>
		/// <para>
		/// Since Blitz3D version 1.97 <c>LoadTexture</c> also supports the loading of DDS
		/// textures. This texture format uses the DXTC compression algorithm which
		/// allows the textures to remain compressed on the video card which can
		/// reduce the video RAM requirements of a program. The buffers of DXTC 
		/// compressed textures are not available meaning locking, drawing, reading
		/// or writing to them will fail.
		/// </para>
		/// </summary>
		/// <param name="file">filename of image file to be used as texture</param>
		/// <param name="flags">optional texture flags</param>
		public static int LoadTexture(String file, int flags) { return bbLoadTexture(file, flags); }
		public static int LoadTexture(String file) { return bbLoadTexture(file, 1); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbLoadAnimTexture(String file, int flags, int frame_width, int frame_height, int first_frame, int frame_count);
		/// <summary>
		/// <para>
		/// Loads an animated texture from an image file and returns the 
		/// texture's handle.
		/// </para>
		/// <para>
		/// Supported file formats include BMP, PNG, TGA, JPG and DDS.
		/// </para>
		/// <para>
		/// Only PNG, TGA and specific formats of DDS support an alpha channel 
		/// which provides per pixel transparency information.
		/// </para>
		/// <para>
		/// See <c>CreateTexture</c> for more detailed descriptions of the texture flags and
		/// the <c>TextureFilter</c> command for an alternative method of setting the texture
		/// flags of a loaded texture based on the texture file's name.
		/// </para>
		/// <para>
		/// The frame_width, frame_height, first_frame and frame_count
		/// parameters determine how Blitz will separate the image file into individual
		/// animation frames.
		/// </para>
		/// <para>
		/// The frames must be drawn in similar sized rectangles
		/// arranged from left to right, top to bottom on the
		/// image source.
		/// </para>
		/// </summary>
		/// <param name="file">name of image file with animation frames laid out in left-right, top-to-bottom order</param>
		/// <param name="flags">optional texture flags</param>
		/// <param name="frame_width">width of each animation frame in pixels</param>
		/// <param name="frame_height">height of each animation frame in pixels</param>
		/// <param name="first_frame">the first frame to be loaded, where 0 is the top left frame in the imagefile</param>
		/// <param name="frame_count">the number of frames to load</param>
		public static int LoadAnimTexture(String file, int flags, int frame_width, int frame_height, int first_frame, int frame_count) { return bbLoadAnimTexture(file, flags, frame_width, frame_height, first_frame, frame_count); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbCreateTexture(int width, int height, int flags, int frames);
		/// <summary>
		/// <para>
		/// Creates a texture and returns its handle.
		/// </para>
		/// <para>
		/// Width and height are the pixel dimensions of the texture. 
		/// </para>
		/// <para>
		/// Note that the size of the actual texture created may be
		/// different from the width and height requested due to the
		/// limitations of various designs of 3D hardware.
		/// </para>
		/// <para>
		/// The optional <c>flags</c> parameter allows you to apply
		/// certain effects to the texture:
		/// </para>
		/// <list type="table">
		/// <listheader><term>Name</term><term>Value</term><term>Description</term></listheader>
		/// <item><description>TX_COLOR</description></item><item><description>1</description></item><item><description>Color (default)</description></item>
		/// <item><description>TX_ALPHA</description></item><item><description>2</description></item><item><description>Alpha</description></item>
		/// <item><description>TX_MASKED</description></item><item><description>4</description></item><item><description>Masked</description></item>
		/// <item><description>TX_MIP</description></item><item><description>8</description></item><item><description>Mipmapped</description></item>
		/// <item><description>TX_CLAMPU</description></item><item><description>16</description></item><item><description>Clamp U</description></item>
		/// <item><description>TX_CLAMPV</description></item><item><description>32</description></item><item><description>Clamp V</description></item>
		/// <item><description>TX_SPHERE</description></item><item><description>64</description></item><item><description>Spherical reflection map</description></item>
		/// <item><description>TX_CUBIC</description></item><item><description>128</description></item><item><description>Cubic environment map</description></item>
		/// <item><description>TX_VRAM</description></item><item><description>256</description></item><item><description>Store texture in vram</description></item>
		/// <item><description>TX_HIGHCOLOR</description></item><item><description>512</description></item><item><description>Force the use of high color textures</description></item>
		/// </list>
		/// <para>
		/// Flags can be added to combine two or more effects, e.g. 3 (1+2) = texture
		/// with color and alpha maps.
		/// </para>
		/// <para>
		/// Color - color map, what you see is what you get.
		/// </para>
		/// <para>
		/// Alpha - alpha channel. If an image contains an alpha map, this will be used to
		/// make certain areas of the texture transparent. Otherwise, the color map 
		/// will be used as an alpha map. With alpha maps, the dark areas always 
		/// equal high-transparency, light areas equal low-transparency.
		/// </para>
		/// <para>
		/// Masked - all areas of a texture colored Black (0,0,0) will be treated
		/// as 100% transparent and not be drawn. Unlike alpha textures, masked 
		/// textures can make use of the zbuffer making them faster and less prone
		/// to depth sorting issues.
		/// </para>
		/// <para>
		/// Mipmapped - low detail versions of the texture are generated for use
		/// at various distances resulting in both smoother filtering and higher
		/// performance rendering.
		/// </para>
		/// <para>
		/// Clamp U - Disables texture wrapping / repeating in the horizontal axis.
		/// </para>
		/// <para>
		/// Clamp V - Disables texture wrapping / repeating in the vertical axis.
		/// </para>
		/// <para>
		/// Spherical environment map - a form of environment mapping. This works by 
		/// taking a single image, and then applying it to a 3D mesh in such a way 
		/// that the image appears to be reflected. When used with a texture that 
		/// contains light sources, it can give some meshes such as a teapot a 
		/// shiny appearance.
		/// </para>
		/// <para>
		/// Cubic environment map - a form of environment mapping. Cube mapping is 
		/// similar to spherical mapping, except it uses six images each representing 
		/// a particular 'face' of an imaginary cube, to give the appearance of an 
		/// image that perfectly reflects its surroundings. 
		/// </para>
		/// <para>
		/// When creating cubic environment maps with the CreateTexture command,
		/// cubemap textures must be square 'power of 2' sizes. See the <c>SetCubeFace</c> 
		/// command for information on how to then draw to the cubemap.
		/// </para>
		/// <para>
		/// When loading cubic environments maps into Blitz using LoadTexture, all 
		/// six images relating to the six faces of the cube must be contained within 
		/// the one texture, and be laid out in a horizontal strip in the following 
		/// order - left, forward, right, backward, up, down.
		/// </para>
		/// <para>
		/// The images comprising the cubemap must all be power of two sizes.
		/// </para>
		/// <para>
		/// Please note that not some older graphics cards do not support cubic mapping.
		/// </para>
		/// <para>
		/// In order to find out if a user's graphics card can support it, use 
		/// <c>GfxDriverCaps3D</c> .
		/// </para>
		/// <para>
		/// See <c>SetCubeFace</c> and <c>SetCubeMode</c> for more information about using cube 
		/// mapping in Blitz3D.
		/// </para>
		/// <para>
		/// Store texture in vram - In some circumstances, this makes for much faster 
		/// dynamic textures - ie. when using CopyRect between two textures. When 
		/// drawing to cube maps in real-time, it is preferable to use this flag.
		/// </para>
		/// <para>
		/// Force the use of high color textures in low bit depth graphics modes.
		/// This is useful for when you are in 16-bit color mode, and wish to 
		/// create/load textures with the alpha flag - it should give better results. 
		/// </para>
		/// <para>
		/// Once you have created a texture, use SetBuffer TextureBuffer to draw to it.
		/// </para>
		/// <para>
		/// However, to display 2D graphics on a texture, it is usually quicker to draw 
		/// to an image and then copy it to the texturebuffer, and to display 3D 
		/// graphics on a texture, your only option is to copy from the backbuffer to 
		/// the texturebuffer.
		/// </para>
		/// </summary>
		/// <param name="width">width of texture</param>
		/// <param name="height">height of texture</param>
		/// <param name="flags">combination of texture flags listed</param>
		/// <param name="frames">no of frames texture will have. Defaults to 1.</param>
		public static int CreateTexture(int width, int height, int flags, int frames) { return bbCreateTexture(width, height, flags, frames); }
		public static int CreateTexture(int width, int height, int flags) { return bbCreateTexture(width, height, flags, 1); }
		public static int CreateTexture(int width, int height) { return bbCreateTexture(width, height, 0, 1); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbFreeTexture(int texture);
		/// <summary>
		/// <para>
		/// Frees a texture's resources from memory.
		/// </para>
		/// <para>
		/// Freeing a texture means you will not be able to use it again; however, 
		/// entities already textured with it will not lose the texture.
		/// </para>
		/// </summary>
		/// <param name="texture">texture handle</param>
		public static int FreeTexture(int texture) { return bbFreeTexture(texture); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbTextureBlend(int texture, int blend);
		/// <summary>
		/// <para>
		/// Sets the blending mode for a texture.
		/// </para>
		/// <para>
		/// The texture blend mode determines how the texture will blend with the 
		/// texture or polygon surface 'below' it.
		/// </para>
		/// <list type="table">
		/// <listheader><term>Mode</term><term>Value</term><term>Description</term></listheader>
		/// <item><description>TX_BLEND_NONE</description></item><item><description>0</description></item><item><description>Do not blend</description></item>
		/// <item><description>TX_BLEND_ALPHA</description></item><item><description>1</description></item><item><description>No blend or Alpha (alpha when texture loaded with alpha flag - not recommended for multitexturing - see below)</description></item>
		/// <item><description>TX_BLEND_MULT</description></item><item><description>2</description></item><item><description>Multiply (default)</description></item>
		/// <item><description>TX_BLEND_ADD</description></item><item><description>3</description></item><item><description>Add</description></item>
		/// <item><description>TX_BLEND_DOT3</description></item><item><description>4</description></item><item><description>Dot3</description></item>
		/// <item><description>TX_BLEND_MULT2</description></item><item><description>5</description></item><item><description>Multiply x 2</description></item>
		/// </list>
		/// <para>
		/// Each of the blend modes are identical to their <c>EntityBlend</c> counterparts.
		/// </para>
		/// <para>
		/// Texture blending in Blitz3D begins with the highest order
		/// texture (the one with the highest index) and blends with 
		/// the next indexed texture:
		/// </para>
		/// <para>
		/// Texture 2 will blend with texture 1.
		/// </para>
		/// <para>
		/// Texture 1 will blend with texture 0.
		/// </para>
		/// <para>
		/// Texture 0 will blend with the polygons of the entity it is applied to.
		/// </para>
		/// <para>
		/// And so on...
		/// </para>
		/// <para>
		/// See the <c>BrushTexture</c> and <c>EntityTexture</c> commands for setting
		/// the index number of a texture.
		/// </para>
		/// <para>
		/// In the case of multitexturing (more than one texture applied to an entity),
		/// it is not recommended you blend textures that have been loaded with the
		/// alpha flag, as this can cause unpredictable results on a variety of different
		/// graphics cards. 
		/// </para>
		/// </summary>
		/// <param name="texture">texture handle.</param>
		/// <param name="blend">blend mode of texture.</param>
		public static int TextureBlend(int texture, int blend) { return bbTextureBlend(texture, blend); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbTextureCoords(int texture, int coords);
		/// <summary>
		/// <para>
		/// Sets the texture coordinate mode for a texture.
		/// </para>
		/// <para>
		/// This determines where the UV values used to look up a texture come from.
		/// </para>
		/// <list type="table">
		/// <listheader><term>Coords</term><term>Description</term></listheader>
		/// <item><description>0</description></item><item><description>UV coordinates are from first UV set in vertices (default)</description></item>
		/// <item><description>1</description></item><item><description>UV coordinates are from second UV set in vertices</description></item>
		/// </list>
		/// </summary>
		/// <param name="texture">texture handle</param>
		/// <param name="coords">coordinate set (0 or 1)</param>
		public static int TextureCoords(int texture, int coords) { return bbTextureCoords(texture, coords); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbScaleTexture(int texture, float u_scale, float v_scale);
		/// <summary>
		/// <para>
		/// Scales a texture by an absolute amount.
		/// </para>
		/// <para>
		/// Effective immediately on all instances of the texture being used.
		/// </para>
		/// </summary>
		/// <param name="texture">texture handle</param>
		/// <param name="u_scale">u scale</param>
		/// <param name="v_scale">v scale</param>
		public static int ScaleTexture(int texture, float u_scale, float v_scale) { return bbScaleTexture(texture, u_scale, v_scale); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbPositionTexture(int texture, float u_position, float v_position);
		/// <summary>
		/// <para>
		/// Positions a texture at an absolute position.
		/// </para>
		/// <para>
		/// Positioning a texture is useful for performing scrolling texture effects,
		/// such as for water etc.
		/// </para>
		/// </summary>
		/// <param name="texture">texture handle</param>
		/// <param name="u_position">u position of texture</param>
		/// <param name="v_position">v position of texture</param>
		public static int PositionTexture(int texture, float u_position, float v_position) { return bbPositionTexture(texture, u_position, v_position); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbRotateTexture(int texture, float angle);
		/// <summary>
		/// <para>
		/// Rotates a texture.
		/// </para>
		/// <para>
		/// This will have an immediate effect on all instances of the texture being used. 
		/// </para>
		/// <para>
		/// Rotating a texture is useful for performing swirling texture effects, such as
		/// for smoke etc.
		/// </para>
		/// </summary>
		/// <param name="texture">texture handle</param>
		/// <param name="angle">absolute angle of texture rotation</param>
		public static int RotateTexture(int texture, float angle) { return bbRotateTexture(texture, angle); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbTextureWidth(int texture);
		/// <summary>
		/// <para>
		/// Returns the width of a texture in pixels.
		/// </para>
		/// </summary>
		/// <param name="texture">texture handle</param>
		public static int TextureWidth(int texture) { return bbTextureWidth(texture); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbTextureHeight(int texture);
		/// <summary>
		/// <para>
		/// Returns the height of a texture in pixels.
		/// </para>
		/// </summary>
		/// <param name="texture">texture handle</param>
		public static int TextureHeight(int texture) { return bbTextureHeight(texture); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbTextureBuffer(int texture, int frame);
		/// <summary>
		/// <para>
		/// Returns the handle of a texture's drawing buffer.
		/// </para>
		/// <para>
		/// This can be used with <c>SetBuffer</c> to perform 2D drawing operations to
		/// the texture, although it's usually faster to draw to an image, and 
		/// then copy the image buffer across to the texture buffer using <c>CopyRect.</c>
		/// </para>
		/// <para>
		/// You cannot render 3D to a texture buffer as <c>RenderWorld</c> only works with
		/// a graphic display's back buffer. To display 3D graphics on a texture, use
		/// <c>CopyRect</c> to copy the contents of the back buffer to a texture buffer
		/// after the call to <c>RenderWorld</c>
		/// </para>
		/// </summary>
		/// <param name="texture">texture handle</param>
		/// <param name="frame">optional texture frame</param>
		public static int TextureBuffer(int texture, int frame) { return bbTextureBuffer(texture, frame); }
		public static int TextureBuffer(int texture) { return bbTextureBuffer(texture, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern String bbTextureName(int texture);
		/// <summary>
		/// <para>
		/// Returns a texture's absolute filename.
		/// </para>
		/// <para>
		/// To find out just the name of the texture, you will need to strip
		/// the path information from the string returned by <c>TextureName.</c>
		/// </para>
		/// </summary>
		/// <param name="texture">a valid texture handle</param>
		public static String TextureName(int texture) { return bbTextureName(texture); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbGetBrushTexture(int brush, int index);
		/// <summary>
		/// <para>
		/// Returns the texture that is applied to the specified brush.
		/// </para>
		/// <para>
		/// The optional <c>index</c> parameter allows you to specify which particular 
		/// texture you'd like returning, if there are more than one textures 
		/// applied to a brush.
		/// </para>
		/// <para>
		/// You should release the texture returned by GetBrushTexture after use 
		/// to prevent leaks! Use <c>FreeTexture</c> to do this.
		/// </para>
		/// <para>
		/// To find out the name of the texture, use <c>TextureName.</c>
		/// </para>
		/// </summary>
		/// <param name="brush">brush handle</param>
		/// <param name="index">optional index of texture applied to brush, from 0..7. Defaults to 0.</param>
		public static int GetBrushTexture(int brush, int index) { return bbGetBrushTexture(brush, index); }
		public static int GetBrushTexture(int brush) { return bbGetBrushTexture(brush, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbTextureFilter(String match_text, int flags);
		/// <summary>
		/// <para>
		/// Adds a texture filter.
		/// </para>
		/// <para>
		/// Any texture files subsequently loaded with <c>LoadTexture,</c> <c>LoadAnimTexture</c> or
		/// as the result of <c>LoadMesh</c> or <c>LoadAnimMesh</c> that contain the text match_text$
		/// in their filename will inherit the specified flags.
		/// </para>
		/// <list type="table">
		/// <listheader><term>Flag</term><term>Value</term><term>Description </term></listheader>
		/// <item><description>TX_COLOR</description></item><item><description>1</description></item><item><description>Color (default)</description></item>
		/// <item><description>TX_ALPHA</description></item><item><description>2</description></item><item><description>Alpha</description></item>
		/// <item><description>TX_MASKED</description></item><item><description>4</description></item><item><description>Masked</description></item>
		/// <item><description>TX_MIP</description></item><item><description>8</description></item><item><description>Mipmapped</description></item>
		/// <item><description>TX_CLAMPU</description></item><item><description>16</description></item><item><description>Clamp U</description></item>
		/// <item><description>TX_CLAMPV</description></item><item><description>32</description></item><item><description>Clamp V</description></item>
		/// <item><description>TX_SPHERE</description></item><item><description>64</description></item><item><description>Spherical reflection map</description></item>
		/// <item><description>TX_CUBIC</description></item><item><description>128</description></item><item><description>Cubic environment map</description></item>
		/// <item><description>TX_VRAM</description></item><item><description>256</description></item><item><description>Store texture in vram</description></item>
		/// <item><description>TX_HIGHCOLOR</description></item><item><description>512</description></item><item><description>Force the use of high color textures</description></item>
		/// </list>
		/// <para>
		/// See <c>CreateTexture</c> for more information on texture flags.
		/// </para>
		/// <para>
		/// By default, the following texture filter is used: 
		/// </para>
		/// <para>
		/// TextureFilter &quot;&quot;,1+8 
		/// </para>
		/// <para>
		/// This means that all loaded textures will have color and be mipmapped by default.
		/// </para>
		/// </summary>
		/// <param name="match_text">text that, if found in texture filename, will activate certain filters</param>
		/// <param name="flags">filter texture flags</param>
		public static int TextureFilter(String match_text, int flags) { return bbTextureFilter(match_text, flags); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbClearTextureFilters();
		/// <summary>
		/// <para>
		/// Clears the current texture filter list.
		/// </para>
		/// <para>
		/// This command must follow any call to Graphics3D which resets the systems
		/// texture flags to their default values which are Color and MipMapped. See
		/// the <c>TextureFilter</c> command for more information.
		/// </para>
		/// </summary>
		public static int ClearTextureFilters() { return bbClearTextureFilters(); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbSetCubeFace(int texture, int face);
		/// <summary>
		/// <para>
		/// Selects a cube face for direct rendering to a texture.
		/// </para>
		/// <list type="table">
		/// <listheader><term>Face</term><term>Value</term><term>Description</term></listheader>
		/// <item><description>CUBEFACE_LEFT</description></item><item><description>0</description></item><item><description>left (negative X) face</description></item>
		/// <item><description>CUBEFACE_FRONT</description></item><item><description>1</description></item><item><description>forward (positive Z) face - this is the default.</description></item>
		/// <item><description>CUBEFACE_RIGHT</description></item><item><description>2</description></item><item><description>right (positive X) face</description></item>
		/// <item><description>CUBEFACE_BACK</description></item><item><description>3</description></item><item><description>backward (negative Z) face</description></item>
		/// <item><description>CUBEFACE_TOP</description></item><item><description>4</description></item><item><description>up (positive Y) face</description></item>
		/// <item><description>CUBEFACE_BOTTOM</description></item><item><description>5</description></item><item><description>down (negative Y) face</description></item>
		/// </list>
		/// <para>
		/// This command should only be used when you wish to draw directly to a
		/// cubemap texture in real-time.
		/// </para>
		/// <para>
		/// Otherwise, just loading a pre-rendered cubemap with a flag of 128 will suffice.
		/// </para>
		/// <para>
		/// To understand how this command works exactly it is important to recognise
		/// that Blitz treats cubemap textures slightly differently to how it treats
		/// other textures. Here's how it works:
		/// </para>
		/// <para>
		/// A cubemap texture in Blitz actually consists of six images, each of which 
		/// must be square 'power' of two size - e.g. 32, 64, 128 etc. Each corresponds
		/// to a particular cube face. These images are stored internally by Blitz, and
		/// the texture handle that is returned by LoadTexture/CreateTexture when 
		/// specifying the cubemap flag, only provides access to one of these six 
		/// images at once (by default the first one, or '1' face).
		/// </para>
		/// <para>
		/// This is why, when loading a cubemap texture into Blitz using
		/// LoadTexture, all the six cubemap images must be laid out in a specific order
		/// (0-5, as described above), in a horizontal strip. Then Blitz takes this
		/// texture and internally converts it into six separate images.
		/// </para>
		/// <para>
		/// So seeing as the texture handle returned by <c>CreateTexture</c> / <c>LoadTexture</c> 
		/// only provides access to one of these images at once (no. 1 by default), 
		/// how do we get access to the other five images? This is where <c>SetCubeFace</c> 
		/// comes in. 
		/// </para>
		/// <para>
		/// It will tell Blitz that whenever you next draw to a cubemap texture, to draw
		/// to the particular image representing the face you have specified with the
		/// <c>face</c> parameter.
		/// </para>
		/// <para>
		/// Now you have the ability to draw to a cubemap in real-time.
		/// </para>
		/// <para>
		/// To give you some idea of how this works in code, see the example included
		/// in the online help. It works by rendering six different views
		/// and copying them to the cubemap texture buffer, using <c>SetCubeFace</c> to specify
		/// which particular cubemap image should be drawn to.
		/// </para>
		/// <para>
		/// All rendering to a texture buffer affects the currently
		/// selected face. Do not change the selected cube face while a buffer is locked.
		/// </para>
		/// <para>
		/// Finally, you may wish to combine the vram 256 flag with the cubic mapping
		/// flag when drawing to cubemap textures for faster access.
		/// </para>
		/// </summary>
		/// <param name="texture">a cubemap type texture</param>
		/// <param name="face">face of cube to select 0..5</param>
		public static int SetCubeFace(int texture, int face) { return bbSetCubeFace(texture, face); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbSetCubeMode(int texture, int mode);
		/// <summary>
		/// <para>
		/// Set the rendering mode of a cubemap texture.
		/// </para>
		/// <list type="table">
		/// <listheader><term>Mode</term><term>Value</term><term>Description</term></listheader>
		/// <item><description>CUBEMODE_SPECULAR</description></item><item><description>1</description></item><item><description>Specular (default) </description></item>
		/// <item><description>CUBEMODE_DIFFUSE</description></item><item><description>2</description></item><item><description>Diffuse</description></item>
		/// <item><description>CUBEMODE_3</description></item><item><description>Refraction</description></item>
		/// </list>
		/// <para>
		/// The available rendering modes are as follows:
		/// </para>
		/// <para>
		/// Specular (default) - use this to give your cubemapped 
		/// objects a shiny effect.
		/// </para>
		/// <para>
		/// Diffuse - use this to give your cubemapped objects a 
		/// non-shiny, realistic lighting effect.
		/// </para>
		/// <para>
		/// Refraction - Good for 'cloaking device' style effects.
		/// </para>
		/// </summary>
		/// <param name="texture">a valid texture handle</param>
		/// <param name="mode">the rendering mode of the cubemap texture:</param>
		public static int SetCubeMode(int texture, int mode) { return bbSetCubeMode(texture, mode); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbCreateBrush(float red, float green, float blue);
		/// <summary>
		/// <para>
		/// Creates a brush with an optional color that can be used with the
		/// <c>PaintEntity</c> and <c>PaintSurface</c> commands.
		/// </para>
		/// <para>
		/// See the <c>Color</c> command for more information on combining
		/// red, green and blue values to define colors. The brush will
		/// default to White if no color is specified.
		/// </para>
		/// <para>
		/// A brush is a collection of properties including color, alpha,
		/// shininess, textures, blend mode and rendering effects.
		/// </para>
		/// <para>
		/// All the properties of a brush are assigned to an entity or
		/// particular surface with the <c>PaintEntity,</c> <c>PaintMesh</c> and 
		/// <c>PaintSurface</c> commands.
		/// </para>
		/// <para>
		/// Painting an entity with a brush can be more efficient than
		/// setting its individual properties with individual calls
		/// to <c>EntityColor,</c> <c>EntityFX,</c> <c>EntityAlpha</c> etc.
		/// </para>
		/// <para>
		/// Brushes are required in order to modify the equivalent
		/// surface properties of meshes that when combined with the
		/// entity properties result in the final rendering properties
		/// of the surface. See <c>PaintSurface</c> for more information.
		/// </para>
		/// </summary>
		/// <param name="red">brush red value</param>
		/// <param name="green">brush green value</param>
		/// <param name="blue">brush blue value</param>
		public static int CreateBrush(float red, float green, float blue) { return bbCreateBrush(red, green, blue); }
		public static int CreateBrush(float red, float green) { return bbCreateBrush(red, green, 255); }
		public static int CreateBrush(float red) { return bbCreateBrush(red, 255, 255); }
		public static int CreateBrush() { return bbCreateBrush(255, 255, 255); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbLoadBrush(String texture_file, int flags, float u_scale, float v_scale);
		/// <summary>
		/// <para>
		/// Creates a brush and loads and assigns a single texture to it with the
		/// specified texture properties.
		/// </para>
		/// <para>
		/// See the <c>CreateTexture</c> command for a discussion of the various texture
		/// flags and their effects and the <c>ScaleTexture</c> command for more information
		/// on texture scales.
		/// </para>
		/// </summary>
		/// <param name="texture_file">filename of texture</param>
		/// <param name="flags">optional texture flags</param>
		/// <param name="u_scale">optional texture horizontal scale</param>
		/// <param name="v_scale">optional texture vertical scale</param>
		public static int LoadBrush(String texture_file, int flags, float u_scale, float v_scale) { return bbLoadBrush(texture_file, flags, u_scale, v_scale); }
		public static int LoadBrush(String texture_file, int flags, float u_scale) { return bbLoadBrush(texture_file, flags, u_scale, 1); }
		public static int LoadBrush(String texture_file, int flags) { return bbLoadBrush(texture_file, flags, 1, 1); }
		public static int LoadBrush(String texture_file) { return bbLoadBrush(texture_file, 1, 1, 1); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbFreeBrush(int brush);
		/// <summary>
		/// <para>
		/// Frees up a brush.
		/// </para>
		/// </summary>
		/// <param name="brush">brush handle</param>
		public static int FreeBrush(int brush) { return bbFreeBrush(brush); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbBrushColor(int brush, float red, float green, float blue);
		/// <summary>
		/// <para>
		/// Modifies the color of a brush.
		/// </para>
		/// <para>
		/// See the <c>Color</c> command for more information on combining
		/// red, green and blue values to define colors.
		/// </para>
		/// <para>
		/// Please note that if <c>EntityFX</c> or <c>BrushFX</c> flag 2 is being used,
		/// brush color will have no effect and vertex colors will be used
		/// instead.
		/// </para>
		/// </summary>
		/// <param name="brush">brush handle</param>
		/// <param name="red">red value of brush</param>
		/// <param name="green">green value of brush</param>
		/// <param name="blue">blue value of brush</param>
		public static int BrushColor(int brush, float red, float green, float blue) { return bbBrushColor(brush, red, green, blue); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbBrushAlpha(int brush, float alpha);
		/// <summary>
		/// <para>
		/// Sets the alpha level of a brush.
		/// </para>
		/// <para>
		/// The alpha<c></c> value should be in the range 0.0 to 1.0.
		/// </para>
		/// <para>
		/// The default brush alpha setting is 1.0. 
		/// </para>
		/// <para>
		/// The alpha level is how transparent an entity is.
		/// </para>
		/// <para>
		/// A value of 1 will mean the entity is non-transparent, i.e. opaque.
		/// </para>
		/// <para>
		/// A value of 0 will mean the entity is completely transparent, i.e. invisible.
		/// </para>
		/// <para>
		/// Values between 0 and 1 will cause varying amount of transparency
		/// accordingly, useful for imitating the look of objects such as glass 
		/// and ice. 
		/// </para>
		/// <para>
		/// A <c>BrushAlpha</c> value of 0 is especially useful as Blitz3D will
		/// not render surfaces painted with such a brush, but will still
		/// involve the entities in collision tests.
		/// </para>
		/// </summary>
		/// <param name="brush">brush handle</param>
		/// <param name="alpha">alpha level of brush</param>
		public static int BrushAlpha(int brush, float alpha) { return bbBrushAlpha(brush, alpha); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbBrushBlend(int brush, int blend);
		/// <summary>
		/// <para>
		/// Sets the blending mode for a brush.
		/// </para>
		/// <list type="table">
		/// <listheader><term>Mode</term><term>Value</term><term>Description</term></listheader>
		/// <item><description>BLEND_NONE</description></item><item><description>0</description></item><item><description>inherit surface blend mode</description></item>
		/// <item><description>BLEND_ALPHA</description></item><item><description>1</description></item><item><description>averages colors based on transparancy (default)</description></item>
		/// <item><description>BLEND_MULT</description></item><item><description>2</description></item><item><description>multiplies colors together</description></item>
		/// <item><description>BLEND_ADD</description></item><item><description>3</description></item><item><description>adds colors together</description></item>
		/// </list>
		/// </summary>
		/// <param name="brush">brush handle</param>
		/// <param name="blend">blend mode</param>
		public static int BrushBlend(int brush, int blend) { return bbBrushBlend(brush, blend); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbBrushFX(int brush, int fx);
		/// <summary>
		/// <para>
		/// Sets miscellaneous effects for a brush.
		/// </para>
		/// <para>
		/// Flags can be added to combine two or more effects. For example,
		/// specifying a flag of 3 (1+2) will result in a full-bright 
		/// vertex-colored brush.
		/// </para>
		/// <list type="table">
		/// <listheader><term>Flag</term><term>Value</term><term>Description</term></listheader>
		/// <item><description>BRUSHFX_NONE</description></item><item><description>0</description></item><item><description>Nothing (default)</description></item>
		/// <item><description>BRUSHFX_FULLBRIGHT</description></item><item><description>1</description></item><item><description>FullBright</description></item>
		/// <item><description>BRUSHFX_VERTEXCOLOR</description></item><item><description>2</description></item><item><description>EnableVertexColors</description></item>
		/// <item><description>BRUSHFX_FLATSHADED</description></item><item><description>4</description></item><item><description>FlatShaded</description></item>
		/// <item><description>BRUSHFX_NOFOG</description></item><item><description>8</description></item><item><description>DisableFog</description></item>
		/// <item><description>BRUSHFX_DOUBLESIDED</description></item><item><description>16</description></item><item><description>DoubleSided</description></item>
		/// <item><description>BRUSHFX_VERTEXALPHA</description></item><item><description>32</description></item><item><description>EnableVertexAlpha</description></item>
		/// </list>
		/// <para>
		/// See the <c>EntityFX</c> command for details on the meaning of each flag.
		/// </para>
		/// </summary>
		/// <param name="brush">brush handle</param>
		/// <param name="fx">effects flags</param>
		public static int BrushFX(int brush, int fx) { return bbBrushFX(brush, fx); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbBrushShininess(int brush, float shininess);
		/// <summary>
		/// <para>
		/// Sets the shininess (specularity) of a brush.
		/// </para>
		/// <para>
		/// The shininess<c></c> value should be in the range 0-1. The default shininess 
		/// setting is 0. 
		/// </para>
		/// <para>
		/// Shininess is how much brighter certain areas of an object will appear to
		/// be when a light is shone directly at them. 
		/// </para>
		/// <para>
		/// Setting a shininess value of 1 for a medium to high poly sphere, combined
		/// with the creation of a light shining in the direction of it, will give it
		/// the appearance of a shiny snooker ball.
		/// </para>
		/// </summary>
		/// <param name="brush">brush handle</param>
		/// <param name="shininess">shininess of brush</param>
		public static int BrushShininess(int brush, float shininess) { return bbBrushShininess(brush, shininess); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbBrushTexture(int brush, int texture, int frame, int index);
		/// <summary>
		/// <para>
		/// Assigns a texture to a brush.
		/// </para>
		/// <para>
		/// The optional <c>frame</c> parameter specifies which animation
		/// frame, if any exist, should be assigned to the brush. 
		/// </para>
		/// <para>
		/// The optional <c>index</c> parameter specifies the texture layer 
		/// that the texture should be assigned to.
		/// </para>
		/// <para>
		/// Brushes have up to eight texture layers, 0-7 inclusive.
		/// </para>
		/// </summary>
		/// <param name="brush">brush handle</param>
		/// <param name="texture">texture handle</param>
		/// <param name="frame">texture frame. Defaults to 0.</param>
		/// <param name="index">brush texture layer. Defaults to 0.</param>
		public static int BrushTexture(int brush, int texture, int frame, int index) { return bbBrushTexture(brush, texture, frame, index); }
		public static int BrushTexture(int brush, int texture, int frame) { return bbBrushTexture(brush, texture, frame, 0); }
		public static int BrushTexture(int brush, int texture) { return bbBrushTexture(brush, texture, 0, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbGetEntityBrush(int entity);
		/// <summary>
		/// <para>
		/// Returns a new brush with the same properties as is currently
		/// applied to the specified entity.
		/// </para>
		/// <para>
		/// See the <c>GetSurfaceBrush</c> function for capturing the properties
		/// of a particualar entities surface which are combined with the
		/// entity properties returned with GetEntityBrush() when Blitz3D
		/// actually renders the surface.
		/// </para>
		/// <para>
		/// Use the <c>FreeBrush</c> command when the newly created brush is no
		/// longer needed.
		/// </para>
		/// <para>
		/// See the <c>GetBrushTexture</c> and <c>TextureName</c> functions for
		/// retrieving details of the brushes texture properties.
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		public static int GetEntityBrush(int entity) { return bbGetEntityBrush(entity); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbGetSurfaceBrush(int surface);
		/// <summary>
		/// <para>
		/// Returns a new brush with the same properties as is currently
		/// applied to the specified surface.
		/// </para>
		/// <para>
		/// See the <c>GetEntityBrush</c> command for capturing entitities default 
		/// surface properties.
		/// </para>
		/// <para>
		/// Use the <c>FreeBrush</c> command when the newly created brush is no
		/// longer needed.
		/// </para>
		/// <para>
		/// See the <c>GetBrushTexture</c> and <c>TextureName</c> functions for
		/// retrieving details of the brushes texture properties.
		/// </para>
		/// </summary>
		/// <param name="surface">surface handle</param>
		public static int GetSurfaceBrush(int surface) { return bbGetSurfaceBrush(surface); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbCountGfxModes3D();
		/// <summary>
		/// <para>
		/// Returns the number of 3D compatible modes available on the selected 3D 
		/// graphics card, and configures the table of information returned by 
		/// <c>GfxModeWidth,</c> <c>GfxModeHeight</c> and <c>GfxModeDepth</c> functions.
		/// </para>
		/// <para>
		/// Use instead of CountGfxModes() to enumerate the available 3D capable
		/// resolutions available for use with the <c>Graphics3D</c> command.
		/// </para>
		/// </summary>
		public static int CountGfxModes3D() { return bbCountGfxModes3D(); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbGfxMode3D(int mode);
		/// <summary>
		/// <para>
		/// Returns True if the specified graphics mode is 3D-capable.
		/// </para>
		/// <para>
		/// This function has been superceeded by the use of <c>CountGfxModes3D()</c>
		/// which removes any non-3D capable modes from the available
		/// mode list.
		/// </para>
		/// </summary>
		/// <param name="mode">graphics mode number from 1 .. CountGfxModes ()</param>
		public static int GfxMode3D(int mode) { return bbGfxMode3D(mode); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbWindowed3D();
		/// <summary>
		/// <para>
		/// Returns <c>True</c> if the current 3D driver is capable of supporting 
		/// <c>Graphics3D</c> in windowed display mode.
		/// </para>
		/// <para>
		/// This function should be used before calling <c>Graphics3D</c> involving
		/// a windowed display.
		/// </para>
		/// <para>
		/// Older generation graphics cards may only support 3D &quot;in a window&quot; 
		/// if the desktop is set to a specific color depth if at all.
		/// </para>
		/// </summary>
		public static int Windowed3D() { return bbWindowed3D(); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbGfxMode3DExists(int width, int height, int depth);
		/// <summary>
		/// <para>
		/// Returns <c>True</c> if the current driver can display 3D graphics at
		/// the specified resolution.
		/// </para>
		/// <para>
		/// Use the <c>GfxMode3DExists</c> command to avoid a possible &quot;Unable
		/// to set Graphics mode&quot; runtime error when calling <c>Graphics3D</c>
		/// which occurs if the user's computer is unable to support 3D
		/// graphics displays at the specified resolution.
		/// </para>
		/// <para>
		/// See <c>CountGfxModes3D</c> for an alternative method of ensuring
		/// the 3D driver supports certain resolution requirements.
		/// </para>
		/// </summary>
		/// <param name="width">width of screen resolution</param>
		/// <param name="height">height of screen resolution</param>
		/// <param name="depth">color depth of screen. 0 = any color depth is OK</param>
		public static int GfxMode3DExists(int width, int height, int depth) { return bbGfxMode3DExists(width, height, depth); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbGfxDriver3D(int driver);
		/// <summary>
		/// <para>
		/// Returns True if the specified graphics driver is 3D-capable.
		/// </para>
		/// <para>
		/// The graphics driver usually corresponds to the number of monitors
		/// connected to the user's system.
		/// </para>
		/// <para>
		/// If GfxDriver3D returns False the specifed driver will be unable
		/// to support any 3D modes and should not be selected with the
		/// <c>SetGfxDriver</c> command.
		/// </para>
		/// <para>
		/// See <c>CountGfxDrivers</c> for more information on multiple monitor systems.
		/// </para>
		/// </summary>
		/// <param name="driver">display driver number to check, from 1 to CountGfxDrivers ()</param>
		public static int GfxDriver3D(int driver) { return bbGfxDriver3D(driver); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbGfxDriverCaps3D();
		/// <summary>
		/// <para>
		/// Returns the 'caps level' of the current graphics driver. Values are:
		/// </para>
		/// <list type="table">
		/// <listheader><term>Level</term><term>Description</term></listheader>
		/// <item><description>100</description></item><item><description>Card supports all 'standard' Blitz3D operations.</description></item>
		/// <item><description>110</description></item><item><description>Card supports all standard ops plus cubic environment mapping.</description></item>
		/// </list>
		/// <para>
		/// The program must already have configured the 3D display by 
		/// successfully calling <c>Graphics3D</c> before calling this function.
		/// </para>
		/// </summary>
		public static int GfxDriverCaps3D() { return bbGfxDriverCaps3D(); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbHWTexUnits();
		/// <summary>
		/// <para>
		/// Returns the number of hardware texturing units available.
		/// </para>
		/// <para>
		/// The result of <c>HWTexUnits</c> is only of interest as a basic
		/// performance indicator of the current graphics driver as
		/// Blitz3D uses multipass rendering techniques when hardware
		/// texturing units are not available.
		/// </para>
		/// <para>
		/// See the <c>BrushTexture</c> command for more information about
		/// working with multitextured surfaces.
		/// </para>
		/// </summary>
		public static int HWTexUnits() { return bbHWTexUnits(); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbVectorYaw(float x, float y, float z);
		/// <summary>
		/// <para>
		/// Returns the yaw value of a vector.
		/// </para>
		/// <para>
		/// Yaw is a common name for rotation around the Y axis or in this
		/// instance the compass heading in degrees if z is north and x is west.
		/// </para>
		/// </summary>
		/// <param name="x">x vector length</param>
		/// <param name="y">y vector length</param>
		/// <param name="z">z vector length</param>
		public static float VectorYaw(float x, float y, float z) { return bbVectorYaw(x, y, z); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbVectorPitch(float x, float y, float z);
		/// <summary>
		/// <para>
		/// Returns the pitch value of a vector.
		/// </para>
		/// <para>
		/// Pitch is a common name for rotation around the X axis or in this
		/// instance the angle the vector is raised if y is up and the distance 
		/// x,z is forwards.
		/// </para>
		/// </summary>
		/// <param name="x">x vector length</param>
		/// <param name="y">y vector length</param>
		/// <param name="z">z vector length</param>
		public static float VectorPitch(float x, float y, float z) { return bbVectorPitch(x, y, z); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbTFormPoint(float x, float y, float z, int source_entity, int dest_entity);
		/// <summary>
		/// <para>
		/// Transforms a point between coordinate systems. 
		/// </para>
		/// <para>
		/// After using <c>TFormPoint</c> the new coordinates can be 
		/// read with <c>TFormedX(),</c> <c>TFormedY()</c> and <c>TFormedZ().</c> 
		/// </para>
		/// <para>
		/// See <c>EntityX()</c> for details about local coordinates. 
		/// </para>
		/// <para>
		/// Consider a sphere built with <c>CreateSphere().</c>
		/// </para>
		/// <para>
		/// The 'north pole' is at (0,1,0).
		/// </para>
		/// <para>
		/// At first, local and global coordinates
		/// are the same. However, as the sphere is moved, 
		/// turned and scaled the global coordinates
		/// of the north pole change but it's always at 
		/// (0,1,0) in the sphere's local space.
		/// </para>
		/// </summary>
		/// <param name="x">x coordinate of a point in 3d space</param>
		/// <param name="y">y coordinate of a point in 3d space</param>
		/// <param name="z">z coordinate of a point in 3d space</param>
		/// <param name="source_entity">handle of source entity, or 0 for 3d world</param>
		/// <param name="dest_entity">handle of destination entity, or 0 for 3d world</param>
		public static int TFormPoint(float x, float y, float z, int source_entity, int dest_entity) { return bbTFormPoint(x, y, z, source_entity, dest_entity); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbTFormVector(float x, float y, float z, int source_entity, int dest_entity);
		/// <summary>
		/// <para>
		/// Transforms a vector between coordinate systems.
		/// </para>
		/// <para>
		/// After using <c>TFormVector</c> the components of the resulting
		/// vector can be read with <c>TFormedX(),</c> <c>TFormedY()</c> and 
		/// <c>TFormedZ().</c>
		/// </para>
		/// <para>
		/// Similar to <c>TFormPoint,</c> but operates on a vector. A vector 
		/// can be thought of as the 'displacement relative to current
		/// location'.
		/// </para>
		/// <para>
		/// For example, the vector (1,2,3) means one step to the right, 
		/// two steps up and three steps forward. 
		/// </para>
		/// <para>
		/// This is analogous to PositionEntity and MoveEntity.
		/// </para>
		/// </summary>
		/// <param name="x">x component of a vector in 3d space</param>
		/// <param name="y">y component of a vector in 3d space</param>
		/// <param name="z">z component of a vector in 3d space</param>
		/// <param name="source_entity">handle of source entity, or 0 for 3d world</param>
		/// <param name="dest_entity">handle of destination entity, or 0 for 3d world</param>
		public static int TFormVector(float x, float y, float z, int source_entity, int dest_entity) { return bbTFormVector(x, y, z, source_entity, dest_entity); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbTFormNormal(float x, float y, float z, int source_entity, int dest_entity);
		/// <summary>
		/// <para>
		/// Transforms a normal between coordinate systems. After 
		/// using <c>TFormNormal</c> the components of the result can be 
		/// read with <c>TFormedX(),</c> <c>TFormedY()</c> and <c>TFormedZ().</c>
		/// </para>
		/// <para>
		/// After the transformation the new vector is
		/// 'normalized', meaning it is scaled to have length 1.
		/// </para>
		/// </summary>
		/// <param name="x">x component of a vector in 3d space</param>
		/// <param name="y">y component of a vector in 3d space</param>
		/// <param name="z">z component of a vector in 3d space</param>
		/// <param name="source_entity">handle of source entity, or 0 for 3d world</param>
		/// <param name="dest_entity">handle of destination entity, or 0 for 3d world</param>
		public static int TFormNormal(float x, float y, float z, int source_entity, int dest_entity) { return bbTFormNormal(x, y, z, source_entity, dest_entity); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbTFormedX();
		/// <summary>
		/// <para>
		/// Returns the X component of the most recent <c>TFormPoint,</c> 
		/// <c>TFormVector</c> or <c>TFormNormal</c> operation.
		/// </para>
		/// </summary>
		public static float TFormedX() { return bbTFormedX(); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbTFormedY();
		/// <summary>
		/// <para>
		/// Returns the Y component of the most recent <c>TFormPoint,</c> 
		/// <c>TFormVector</c> or <c>TFormNormal</c> operation.
		/// </para>
		/// </summary>
		public static float TFormedY() { return bbTFormedY(); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbTFormedZ();
		/// <summary>
		/// <para>
		/// Returns the Z component of the most recent <c>TFormPoint,</c> 
		/// <c>TFormVector</c> or <c>TFormNormal</c> operation.
		/// </para>
		/// </summary>
		public static float TFormedZ() { return bbTFormedZ(); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbPositionEntity(int entity, float x, float y, float z, int globalspace);
		/// <summary>
		/// <para>
		/// Position an entity at the position defined by the 3D
		/// coordinate (x,y,z) either relative to its parent 
		/// position and orientation or optionally in world
		/// coordinates.
		/// </para>
		/// <para>
		/// In Blitz3D an entity typically faces towards the +z axis,
		/// the y axis is the height of the entity and its left / 
		/// right position is the x axis.
		/// </para>
		/// <para>
		/// In particular all entities including cameras are created
		/// so the x axis points right, the y axis up and the
		/// z axis forwards.
		/// </para>
		/// <para>
		/// A child entity (one that is created with another as
		/// its parent) will by default position itself in it's
		/// parent's space unless the optional global of
		/// <c>PositionEntity</c> is set to True in which case the
		/// entity is positioned relative to the global axis
		/// not its parent.
		/// </para>
		/// <para>
		/// To move an entity a relative amount in respect to
		/// its current position see the <c>MoveEntity</c>
		/// and <c>TranslateEntity</c> commands.
		/// </para>
		/// <para>
		/// See the <c>UpdateWorld</c> command for details regarding
		/// the effect of any collisions that may occur due to
		/// the requested repositioning.
		/// </para>
		/// </summary>
		/// <param name="entity">name of entity to be positioned</param>
		/// <param name="x">x co-ordinate that entity will be positioned at</param>
		/// <param name="y">y co-ordinate that entity will be positioned at</param>
		/// <param name="z">z co-ordinate that entity will be positioned at</param>
		/// <param name="globalspace"><c>False</c> (default) for parent space <c>True</c> for global space</param>
		public static int PositionEntity(int entity, float x, float y, float z, int globalspace) { return bbPositionEntity(entity, x, y, z, globalspace); }
		public static int PositionEntity(int entity, float x, float y, float z) { return bbPositionEntity(entity, x, y, z, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbMoveEntity(int entity, float x, float y, float z);
		/// <summary>
		/// <para>
		/// Moves an entity relative to its current position and orientation.
		/// </para>
		/// <para>
		/// Typically the x,y,z values for a 'Z facing model' are:
		/// </para>
		/// <list type="table">
		/// <listheader><term>Axis</term><term>Direction</term></listheader>
		/// <item><description>X</description></item><item><description>Right Left</description></item>
		/// <item><description>Y</description></item><item><description>Up Down</description></item>
		/// <item><description>Z</description></item><item><description>Forward Backwards </description></item>
		/// </list>
		/// <para>
		/// For movement that ignores the entities orientation see the
		/// <c>TranslateEntity</c> command.
		/// </para>
		/// <para>
		/// To position an entity at an absolute location in parent
		/// or world space see the <c>PositionEntity</c> command.
		/// </para>
		/// <para>
		/// See the <c>UpdateWorld</c> command for details regarding
		/// the effect of any collisions that may occur due to
		/// the requested repositioning.
		/// </para>
		/// </summary>
		/// <param name="entity">name of entity to be moved</param>
		/// <param name="x">x amount that entity will be moved by</param>
		/// <param name="y">y amount that entity will be moved by</param>
		/// <param name="z">z amount that entity will be moved by</param>
		public static int MoveEntity(int entity, float x, float y, float z) { return bbMoveEntity(entity, x, y, z); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbTranslateEntity(int entity, float x, float y, float z, int globalspace);
		/// <summary>
		/// <para>
		/// Translates an entity relative to its current position 
		/// in the direction specified in either parent or global
		/// space. The direction vector by default is interpreted
		/// as being in parent space.
		/// </para>
		/// <para>
		/// <c>TranslateEntity</c> is an alternative to <c>MoveEntity</c>
		/// when an entity must be moved in a global direction
		/// such as straight down for gravity.
		/// </para>
		/// <para>
		/// Unlike <c>MoveEntity</c> an entities orientation is ignored
		/// in the resulting calculation.
		/// </para>
		/// <para>
		/// To move an entity with coordinates that represent
		/// a vector relative to its own orientation use the
		/// <c>MoveEntity</c> command.
		/// </para>
		/// <para>
		/// See the <c>UpdateWorld</c> command for details regarding
		/// the effect of any collisions that may occur due to
		/// the requested repositioning.
		/// </para>
		/// </summary>
		/// <param name="entity">name of entity to be translated</param>
		/// <param name="x">x amount that entity will be translated by</param>
		/// <param name="y">y amount that entity will be translated by</param>
		/// <param name="z">z amount that entity will be translated by</param>
		/// <param name="globalspace"><c>False</c> (default) for parent space <c>True</c> for global space</param>
		public static int TranslateEntity(int entity, float x, float y, float z, int globalspace) { return bbTranslateEntity(entity, x, y, z, globalspace); }
		public static int TranslateEntity(int entity, float x, float y, float z) { return bbTranslateEntity(entity, x, y, z, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbEntityX(int entity, int globalspace);
		/// <summary>
		/// <para>
		/// Returns the X component of the entities current position
		/// in local or optionally global space.
		/// </para>
		/// <para>
		/// The X axis is tradionally the left to right axis in Blitz3D
		/// space when facing towards Z either from the parents point
		/// of view or optionally from the center of the world.
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		/// <param name="globalspace"><c>True</c> to return global coordinate <c>False</c> to return local coordinate</param>
		public static float EntityX(int entity, int globalspace) { return bbEntityX(entity, globalspace); }
		public static float EntityX(int entity) { return bbEntityX(entity, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbEntityY(int entity, int globalspace);
		/// <summary>
		/// <para>
		/// Returns the Y component of the entities current position
		/// in local or optionally global space.
		/// </para>
		/// <para>
		/// The Y axis is tradionally the down to up axis in Blitz3D
		/// space when facing towards Z either from the parents point
		/// of view or optionally from the center of the world.
		/// </para>
		/// </summary>
		/// <param name="entity">handle of Loaded or Created Entity</param>
		/// <param name="globalspace"><c>True</c> to return global coordinate <c>False</c> to return local coordinate</param>
		public static float EntityY(int entity, int globalspace) { return bbEntityY(entity, globalspace); }
		public static float EntityY(int entity) { return bbEntityY(entity, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbEntityZ(int entity, int globalspace);
		/// <summary>
		/// <para>
		/// Returns the Z component of the entities current position
		/// in local or optionally global space.
		/// </para>
		/// <para>
		/// The Z axis is tradionally the from behind to infront axis 
		/// in Blitz3D space from the parents current point of view 
		/// or optionally from the center of the world.
		/// </para>
		/// </summary>
		/// <param name="entity">handle of Loaded or Created Entity</param>
		/// <param name="globalspace"><c>True</c> to return global coordinate <c>False</c> to return local coordinate</param>
		public static float EntityZ(int entity, int globalspace) { return bbEntityZ(entity, globalspace); }
		public static float EntityZ(int entity) { return bbEntityZ(entity, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbRotateEntity(int entity, float pitch, float yaw, float roll, int globalspace);
		/// <summary>
		/// <para>
		/// Rotates an entity relative to its parent's orientation or if
		/// specified in relation to the global axis.
		/// </para>
		/// <list type="table">
		/// <listheader><term>Name</term><term>Rotation Axis</term><term>Description</term></listheader>
		/// <item><description>Pitch</description></item><item><description>Around x axis</description></item><item><description>Equivalent to tilting forward/backwards</description></item>
		/// <item><description>Yaw</description></item><item><description>Around y axis</description></item><item><description> Equivalent to turning left/right</description></item>
		/// <item><description>Roll</description></item><item><description>Around z axis</description></item><item><description> Equivalent to tilting left/right</description></item>
		/// </list>
		/// <para>
		/// See the <c>TurnEntity</c> command for rotating entities starting
		/// from their current position.
		/// </para>
		/// </summary>
		/// <param name="entity">name of the entity to be rotated</param>
		/// <param name="pitch">angle in degrees of rotation around x</param>
		/// <param name="yaw">angle in degrees of rotation around y</param>
		/// <param name="roll">angle in degrees of rotation around z</param>
		/// <param name="globalspace"><c>True</c> to rotate in world space <c>False</c> (default) for parent space</param>
		public static int RotateEntity(int entity, float pitch, float yaw, float roll, int globalspace) { return bbRotateEntity(entity, pitch, yaw, roll, globalspace); }
		public static int RotateEntity(int entity, float pitch, float yaw, float roll) { return bbRotateEntity(entity, pitch, yaw, roll, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbTurnEntity(int entity, float pitch, float yaw, float roll, int globalspace);
		/// <summary>
		/// <para>
		/// Turns an entity relative to its current rotatation.
		/// </para>
		/// <list type="table">
		/// <listheader><term>Name</term><term>Rotation Axis</term><term>Description</term></listheader>
		/// <item><description>Pitch</description></item><item><description>Around x axis</description></item><item><description>Equivalent to tilting forward/backwards</description></item>
		/// <item><description>Yaw</description></item><item><description>Around y axis</description></item><item><description> Equivalent to turning left/right</description></item>
		/// <item><description>Roll</description></item><item><description>Around z axis</description></item><item><description> Equivalent to tilting left/right</description></item>
		/// </list>
		/// <para>
		/// Unlike <c>RotateEntity</c> the <c>pitch,</c> <c>yaw</c> and <c>roll</c> rotations are applied
		/// to the object starting from its current rotation.
		/// </para>
		/// </summary>
		/// <param name="entity">name of entity to be rotated</param>
		/// <param name="pitch">angle in degrees that entity will be pitched</param>
		/// <param name="yaw">angle in degrees that entity will be yawed</param>
		/// <param name="roll">angle in degrees that entity will be rolled</param>
		/// <param name="globalspace"><c>True</c> to rotate in world space <c>False</c> for parent space (default)</param>
		public static int TurnEntity(int entity, float pitch, float yaw, float roll, int globalspace) { return bbTurnEntity(entity, pitch, yaw, roll, globalspace); }
		public static int TurnEntity(int entity, float pitch, float yaw, float roll) { return bbTurnEntity(entity, pitch, yaw, roll, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbPointEntity(int entity, int target, float roll);
		/// <summary>
		/// <para>
		/// Points one entity at another by adjusting its pitch
		/// and yaw rotations.
		/// </para>
		/// <para>
		/// The optional <c>roll</c> parameter allows you to specify a 
		/// rotation around the z axis to complete the rotation.
		/// </para>
		/// <para>
		/// Use the <c>AlignToVector</c> command for aiming an entity
		/// (typically its z axis) using a specified alignment 
		/// vector and smoothing rate.
		/// </para>
		/// <para>
		/// Invisible pivot entities make useful targets for pointing
		/// an entity towards any arbitrary direction.
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		/// <param name="target">target entity handle</param>
		/// <param name="roll">roll angle of entity</param>
		public static int PointEntity(int entity, int target, float roll) { return bbPointEntity(entity, target, roll); }
		public static int PointEntity(int entity, int target) { return bbPointEntity(entity, target, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbAlignToVector(int entity, float vector_x, float vector_y, float vector_z, int axis, float rate);
		/// <summary>
		/// <para>
		/// Aligns an entity axis to a vector.
		/// </para>
		/// <para>
		/// Rate should be in the range 0 to 1, 0.1 for smooth transition and 1.0 for 'snap' 
		/// transition. Defaults to 1.0. 
		/// </para>
		/// <list type="table">
		/// <listheader><term>Axis</term><term>Description</term></listheader>
		/// <item><description>1</description></item><item><description>X axis</description></item>
		/// <item><description>2</description></item><item><description>Y axis</description></item>
		/// <item><description>3</description></item><item><description>Z axis</description></item>
		/// </list>
		/// <para>
		/// The <c>AlignToVector</c> command offers an advanced alternative to the
		/// <c>RotateEntity,</c> <c>TurnEntity</c> and <c>PointEntity</c> commands. <c>AlignToVector</c>
		/// provides a method of steering entities by turning them so a
		/// specified axis aligns to a precalculated directions in an optionally
		/// smooth manner.
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		/// <param name="vector_x">vector x</param>
		/// <param name="vector_y">vector y</param>
		/// <param name="vector_z">vector z</param>
		/// <param name="axis">axis of entity that will be aligned to vector</param>
		/// <param name="rate">rate at which entity is aligned from current orientation to vector orientation.</param>
		public static int AlignToVector(int entity, float vector_x, float vector_y, float vector_z, int axis, float rate) { return bbAlignToVector(entity, vector_x, vector_y, vector_z, axis, rate); }
		public static int AlignToVector(int entity, float vector_x, float vector_y, float vector_z, int axis) { return bbAlignToVector(entity, vector_x, vector_y, vector_z, axis, 1.0f); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbEntityRoll(int entity, int globalspace);
		/// <summary>
		/// <para>
		/// Returns the roll angle of an entity.
		/// </para>
		/// <para>
		/// The roll angle is also the z angle
		/// of an entity.
		/// </para>
		/// </summary>
		/// <param name="entity">name of entity that will have roll angle returned</param>
		/// <param name="globalspace">true if the roll angle returned should be relative to 0 rather than a parent entities roll angle. False by default.</param>
		public static float EntityRoll(int entity, int globalspace) { return bbEntityRoll(entity, globalspace); }
		public static float EntityRoll(int entity) { return bbEntityRoll(entity, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbEntityYaw(int entity, int globalspace);
		/// <summary>
		/// <para>
		/// Returns the yaw angle of an entity.
		/// </para>
		/// <para>
		/// The yaw angle is also the y angle of an entity.
		/// </para>
		/// </summary>
		/// <param name="entity">name of entity that will have yaw angle returned</param>
		/// <param name="globalspace">true if the yaw angle returned should be relative to 0 rather than a parent entities yaw angle. False by default.</param>
		public static float EntityYaw(int entity, int globalspace) { return bbEntityYaw(entity, globalspace); }
		public static float EntityYaw(int entity) { return bbEntityYaw(entity, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbEntityPitch(int entity, int globalspace);
		/// <summary>
		/// <para>
		/// Returns the pitch angle of an entity.
		/// </para>
		/// <para>
		/// The pitch angle is also the x angle of an entity.
		/// </para>
		/// </summary>
		/// <param name="entity">name of entity that will have pitch angle returned</param>
		/// <param name="globalspace">true if the pitch angle returned should be relative to 0 rather than a parent entities pitch angle. False by default.</param>
		public static float EntityPitch(int entity, int globalspace) { return bbEntityPitch(entity, globalspace); }
		public static float EntityPitch(int entity) { return bbEntityPitch(entity, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbGetMatElement(int entity, int row, int column);
		/// <summary>
		/// <para>
		/// Returns the value of an element from within an entities transformation
		/// matrix.
		/// </para>
		/// <para>
		/// The transformation matrix is what is used by Blitz internally
		/// to position, scale and rotate entities.
		/// </para>
		/// <para>
		/// GetMatElement is intended for
		/// use by advanced users only.
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		/// <param name="row">matrix row index</param>
		/// <param name="column">matrix column index</param>
		public static float GetMatElement(int entity, int row, int column) { return bbGetMatElement(entity, row, column); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbScaleEntity(int entity, float x_scale, float y_scale, float z_scale, int globalspace);
		/// <summary>
		/// <para>
		/// Scales the size of an entity a scalar amount in each axis.
		/// </para>
		/// <para>
		/// The values 1,1,1 are the default scale of a newly created
		/// entity which has no affect on the entity size.
		/// </para>
		/// <para>
		/// A scalar values of 2 will double the size of an entity in the
		/// specified axis, 0.5 will halve the size.
		/// </para>
		/// <para>
		/// Negative scalar values invert that entities axis and may
		/// result in reversed facing surfaces. 
		/// </para>
		/// <para>
		/// A global value of False is default and multiplies the specified
		/// scale with the entity parent's scale. A global value of <c>True</c>
		/// ignores the scale of the parent.
		/// </para>
		/// <para>
		/// A scale value of 0,0,0 should be avoided as it can produce
		/// dramatic performance problems on some computers due to the
		/// infinite nature of the surface normals it produces.
		/// </para>
		/// </summary>
		/// <param name="entity">name of the entity to be scaled</param>
		/// <param name="x_scale">x scalar value</param>
		/// <param name="y_scale">y scalar value</param>
		/// <param name="z_scale">z scalar value</param>
		/// <param name="globalspace">True means relative to world not parent's scale</param>
		public static int ScaleEntity(int entity, float x_scale, float y_scale, float z_scale, int globalspace) { return bbScaleEntity(entity, x_scale, y_scale, z_scale, globalspace); }
		public static int ScaleEntity(int entity, float x_scale, float y_scale, float z_scale) { return bbScaleEntity(entity, x_scale, y_scale, z_scale, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbEntityParent(int entity, int parent, int globalspace);
		/// <summary>
		/// <para>
		/// Attaches an entity to a parent.
		/// </para>
		/// <para>
		/// Parent may be 0, in which case the entity will have no parent. 
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		/// <param name="parent">parent entity handle</param>
		/// <param name="globalspace">true for the child entity to retain its global position and orientation. Defaults to true.</param>
		public static int EntityParent(int entity, int parent, int globalspace) { return bbEntityParent(entity, parent, globalspace); }
		public static int EntityParent(int entity, int parent) { return bbEntityParent(entity, parent, 1); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbGetParent(int entity);
		/// <summary>
		/// <para>
		/// Returns an entity's parent.
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		public static int GetParent(int entity) { return bbGetParent(entity); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbCountChildren(int entity);
		/// <summary>
		/// <para>
		/// Returns the number of children of an entity.
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		public static int CountChildren(int entity) { return bbCountChildren(entity); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbGetChild(int entity, int index);
		/// <summary>
		/// <para>
		/// Returns a particular child of a specified entity based on a valid index which
		/// should be in the range 1...CountChildren( entity ) inclusive.
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		/// <param name="index">index of child entity.</param>
		public static int GetChild(int entity, int index) { return bbGetChild(entity, index); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbFindChild(int entity, String child_name);
		/// <summary>
		/// <para>
		/// Searches all the children and descendants of those children for an entity
		/// with a name matching child_name$.
		/// </para>
		/// <para>
		/// Returns the handle of the first entity with a matching name or 0 if none
		/// found.
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		/// <param name="child_name">child name to find within entity</param>
		public static int FindChild(int entity, String child_name) { return bbFindChild(entity, child_name); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbShowEntity(int entity);
		/// <summary>
		/// <para>
		/// Shows an entity. Very
		/// much the opposite of HideEntity.
		/// </para>
		/// <para>
		/// Once an entity has been hidden using <c>HideEntity,</c> use <c>ShowEntity</c> to make 
		/// it visible and involved in collisions again.
		/// </para>
		/// <para>
		/// Note that ShowEntity has no effect if the enitities' parent object
		/// is hidden.
		/// </para>
		/// <para>
		/// Entities are shown by default after creating/loading them,
		/// so you should only need to use ShowEntity after using HideEntity. 
		/// </para>
		/// <para>
		/// ShowEntity affects the specified entity only - child entities are not affected.
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		public static int ShowEntity(int entity) { return bbShowEntity(entity); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbHideEntity(int entity);
		/// <summary>
		/// <para>
		/// Hides an entity, so that it is no longer visible, and is no longer
		/// involved in collisions.
		/// </para>
		/// <para>
		/// The main purpose of hide entity is to allow
		/// you to create entities at the beginning of a program, hide them, then copy
		/// them and show as necessary in the main game. This is more efficient than
		/// creating entities mid-game. 
		/// </para>
		/// <para>
		/// If you wish to hide an entity so that it
		/// is no longer visible but still involved in collisions, then use EntityAlpha
		/// 0 instead. This will make an entity completely transparent. 
		/// </para>
		/// <para>
		/// HideEntity
		/// affects the specified entity and all of its child entities, if any exist.
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		public static int HideEntity(int entity) { return bbHideEntity(entity); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbEntityAutoFade(int entity, float near, float far);
		/// <summary>
		/// <para>
		/// Enables auto fading for an entity.
		/// </para>
		/// <para>
		/// <c>EntityAutoFade</c> causes an entities alpha level to be adjusted at
		/// distances between near and far to create a 'fade-in' effect.
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		/// <param name="near">distance from camera when entity begins to fade</param>
		/// <param name="far">distance from camera entity becomes completely invisible</param>
		public static int EntityAutoFade(int entity, float near, float far) { return bbEntityAutoFade(entity, near, far); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbEntityOrder(int entity, int order);
		/// <summary>
		/// <para>
		/// Sets the drawing order for
		/// an entity.
		/// </para>
		/// <para>
		/// An order value of 0 will mean the entity is drawn normally.
		/// A value greater than 0 will mean that entity is drawn first, behind everything
		/// else. A value less than 0 will mean the entity is drawn last, in front
		/// of everything else. 
		/// </para>
		/// <para>
		/// Setting an entities order to non-0 also disables
		/// z-buffering for the entity, so should be only used for simple, convex entities
		/// like skyboxes, sprites etc. 
		/// </para>
		/// <para>
		/// EntityOrder affects the specified entity
		/// but none of its child entities, if any exist. 
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		/// <param name="order">order that entity will be drawn in</param>
		public static int EntityOrder(int entity, int order) { return bbEntityOrder(entity, order); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbFreeEntity(int EntityHandle);
		/// <summary>
		/// <para>
		/// FreeEntity will free up the internal resources associated
		/// with a particular entity and remove it from the scene.
		/// </para>
		/// <para>
		/// Handle must be valid entity hanlde such as returned by 
		/// function such as CreateCube(), CreateLight(), LoadMesh() etc.
		/// </para>
		/// <para>
		/// This command will also free all children entities parented 
		/// to the entity.
		/// </para>
		/// <para>
		/// See the <c>ClearWorld</c> command for freeing all entities with
		/// a single call.
		/// </para>
		/// <para>
		/// Any references to the entity or its children become
		/// invalid after a call to <c>FreeEntity</c> so care should be
		/// taken as any subsquent use of a handle to a freed 
		/// entity will cause a runtime error.
		/// </para>
		/// </summary>
		/// <param name="EntityHandle">entity handle</param>
		public static int FreeEntity(int EntityHandle) { return bbFreeEntity(EntityHandle); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbEntityColor(int entity, float red, float green, float blue);
		/// <summary>
		/// <para>
		/// Sets the color of an entity.
		/// </para>
		/// <para>
		/// See the <c>Color</c> command for more information on combining
		/// red, green and blue values to define colors.
		/// </para>
		/// <para>
		/// The <c>PaintEntity</c> command can also be used to set the color
		/// properties of individual entities.
		/// </para>
		/// <para>
		/// The <c>PaintSurface</c> command is used to set the color properties
		/// of the individual surfaces of a mesh.
		/// </para>
		/// <para>
		/// The final render color of a surface of an entity mesh is calculated
		/// by multiplying the entity color with the surface color. 
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		/// <param name="red">red value of entity</param>
		/// <param name="green">green value of entity</param>
		/// <param name="blue">blue value of entity</param>
		public static int EntityColor(int entity, float red, float green, float blue) { return bbEntityColor(entity, red, green, blue); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbEntityAlpha(int entity, float alpha);
		/// <summary>
		/// <para>
		/// Sets the alpha or translucent level of an entity.
		/// </para>
		/// <para>
		/// The <c>alpha</c> value should be between 0.0 and 1.0 which
		/// correspond to the range from transparent 
		/// (effectively invisible) to totally opaque:
		/// </para>
		/// <list type="table">
		/// <listheader><term>Alpha Value</term><term>Effect</term></listheader>
		/// <item><description>0.0</description></item><item><description>invisible</description></item>
		/// <item><description>0.25</description></item><item><description>very transparent</description></item>
		/// <item><description>0.50</description></item><item><description>semi transparent</description></item>
		/// <item><description>0.75</description></item><item><description>slightly transparent</description></item>
		/// <item><description>1.00</description></item><item><description>completely opaque</description></item>
		/// </list>
		/// <para>
		/// Unlike <c>HideEntity</c> an entity made invisible with
		/// an <c>alpha</c> of 0.0 still participates in any collisions.
		/// </para>
		/// <para>
		/// The default <c>alpha</c> level of an entity is 1.0.
		/// </para>
		/// <para>
		/// Use the <c>BrushAlpha</c> and <c>PaintSurface</c> commands for
		/// affecting transparency on a surface by surface basis.
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		/// <param name="alpha">alpha level of entity</param>
		public static int EntityAlpha(int entity, float alpha) { return bbEntityAlpha(entity, alpha); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbEntityShininess(int entity, float shininess);
		/// <summary>
		/// <para>
		/// Sets the shininess (specularity index) of an entity.
		/// </para>
		/// <para>
		/// The shininess<c></c> value should be a floating point number in the 
		/// range 0.0-1.0. The default shininess setting is 0.0. 
		/// </para>
		/// <para>
		/// Shininess is the extra brightness that appears on a surface
		/// when it is oriented to reflect light directly towards the 
		/// camera.
		/// </para>
		/// <para>
		/// A low <c>shininess</c> produces a dull non reflective surface while
		/// a high <c>shininess</c> approaching 1.0 will make a surface appear
		/// polished and shiny.
		/// </para>
		/// <para>
		/// Use the <c>BrushShininess</c> and <c>PaintSurface</c> commands for
		/// affecting shininess on a surface by surface basis.
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		/// <param name="shininess">shininess of entity</param>
		public static int EntityShininess(int entity, float shininess) { return bbEntityShininess(entity, shininess); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbEntityTexture(int entity, int texture, int frame, int index);
		/// <summary>
		/// <para>
		/// Applies a texture to an entity.
		/// </para>
		/// <para>
		/// The optional <c>frame</c> parameter specifies which texture
		/// animation frame should be used as the texture. 
		/// </para>
		/// <para>
		/// The <c>index</c> parameter specifies an optional texturing channel
		/// when using multitexturing. See the <c>TextureBlend</c> command for
		/// more details on mixing multiple textures on the same surface.
		/// </para>
		/// <para>
		/// Texturing requires the use of a valid texture returned by
		/// the <c>CreateTexture</c> or <c>LoadTexture</c> functions and a mesh based
		/// entity with texturing coordinates assigned to its vertices.
		/// </para>
		/// <para>
		/// Primitives created with <c>CreateCube,</c> <c>CreatePlane,</c> 
		/// <c>CreateSphere</c> etc. contain texturing information known
		/// as UV coordinates. Howeever model files and surfaces 
		/// created programatically may be missing this information
		/// and will consequently fail to display textures correctly
		/// on their surfaces.
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		/// <param name="texture">texture handle</param>
		/// <param name="frame">frame of texture. Defaults to 0.</param>
		/// <param name="index">index number of texture. Should be in the range to 0-7. Defaults to 0.</param>
		public static int EntityTexture(int entity, int texture, int frame, int index) { return bbEntityTexture(entity, texture, frame, index); }
		public static int EntityTexture(int entity, int texture, int frame) { return bbEntityTexture(entity, texture, frame, 0); }
		public static int EntityTexture(int entity, int texture) { return bbEntityTexture(entity, texture, 0, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbEntityBlend(int entity, int blend);
		/// <summary>
		/// <para>
		/// Sets the blending mode of an entity.
		/// </para>
		/// <para>
		/// A blending mode determines the way in which the color and 
		/// alpha (RGBA) on an entity's surface (source) is combined 
		/// with the color of the background (destination) during rendering.
		/// </para>
		/// <para>
		/// The possible blend modes are:
		/// </para>
		/// <list type="table">
		/// <listheader><term>Name</term><term>Value</term><term>Description</term></listheader>
		/// <item><description>BLEND_ALPHA</description></item><item><description>1</description></item><item><description>Combines <c>alpha</c> amount of src with 1-<c>alpha</c> of dest</description></item><item><description>most things</description></item>
		/// <item><description>BLEND_MULT</description></item><item><description>2</description></item><item><description>Blends src color with dest</description></item><item><description>lightmaps</description></item>
		/// <item><description>BLEND_ADD</description></item><item><description>Adds src color to dest</description></item><item><description>explosions lasers etc.</description></item>
		/// </list>
		/// <para>
		/// Alpha - blends the pixels according to the Alpha value. This is 
		/// roughly done to the formula:
		/// </para>
		/// <para>
		/// Rr = ( An * Rn ) + ( ( 1.0 - An ) * Ro )
		/// </para>
		/// <para>
		/// Gr = ( An * Gn ) + ( ( 1.0 - An ) * Go )
		/// </para>
		/// <para>
		/// Br = ( An * Bn ) + ( ( 1.0 - An ) * Bo )
		/// </para>
		/// <para>
		/// Where R = Red, G = Green, B = Blue, n = new pixel color values, 
		/// r = resultant color values, o = old pixel color values.
		/// </para>
		/// <para>
		/// Alpha blending is the default blending mode and is used
		/// with most world objects.
		/// </para>
		/// <para>
		/// Multiply - darkens the underlying pixels. If you think of each RGB value as 
		/// being on a scale from 0% to 100%, where 0 = 0% and 255 = 100%, the multiply 
		/// blend mode will multiply the red, green and blue values individually together 
		/// in order to get the new RGB value, roughly according to:
		/// </para>
		/// <para>
		/// Rr = ( ( Rn / 255.0 ) * ( Ro / 255.0 ) ) * 255.0
		/// </para>
		/// <para>
		/// Gr = ( ( Gn / 255.0 ) * ( Go / 255.0 ) ) * 255.0
		/// </para>
		/// <para>
		/// Br = ( ( Bn / 255.0 ) * ( Bo / 255.0 ) ) * 255.0
		/// </para>
		/// <para>
		/// The alpha value has no effect with multiplicative blending.
		/// </para>
		/// <para>
		/// Blending a RGB value of 255, 255, 255 will make no difference, while an 
		/// RGB value of 128, 128, 128 will darken the pixels by a factor of 2 and an 
		/// RGB value of 0, 0, 0 will completely blacken out the resultant pixels.
		/// </para>
		/// <para>
		/// An RGB value of 0, 255, 255 will remove the red component of the underlying 
		/// pixel while leaving the other color values untouched.
		/// </para>
		/// <para>
		/// Multiply blending is most often used for lightmaps, shadows or anything else 
		/// that needs to 'darken' the resultant pixels.
		/// </para>
		/// <para>
		/// Add - additive blending will add the new color values to the old, roughly
		/// according to:
		/// </para>
		/// <para>
		/// Rr = ( Rn * An ) + Ro
		/// </para>
		/// <para>
		/// Gr = ( Gn * An ) + Go
		/// </para>
		/// <para>
		/// Br = ( Bn * An ) + Bo
		/// </para>
		/// <para>
		/// The resultant RGB values are clipped out at 255, meaning
		/// that multiple additive effects can quickly cause visible 
		/// banding from smooth gradients.
		/// </para>
		/// <para>
		/// Additive blending is extremely useful for effects such as
		/// laser shots and fire.
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		/// <param name="blend">blend mode of the entity.</param>
		public static int EntityBlend(int entity, int blend) { return bbEntityBlend(entity, blend); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbEntityFX(int entity, int fx);
		/// <summary>
		/// <para>
		/// Sets miscellaneous effects for an entity.
		/// </para>
		/// <list type="table">
		/// <listheader><term>Flag</term><term>Value</term><term>Description</term></listheader>
		/// <item><description>FX_NONE</description></item><item><description>0</description></item><item><description>Nothing (default)</description></item>
		/// <item><description>FX_FULLBRIGHT</description></item><item><description>1</description></item><item><description>FullBright</description></item>
		/// <item><description>FX_VERTEXCOLOR</description></item><item><description>2</description></item><item><description>EnableVertexColors</description></item>
		/// <item><description>FX_FLATSHADED</description></item><item><description>4</description></item><item><description>FlatShaded</description></item>
		/// <item><description>FX_NOFOG</description></item><item><description>8</description></item><item><description>DisableFog</description></item>
		/// <item><description>FX_DOUBLESIDED</description></item><item><description>16</description></item><item><description>DoubleSided</description></item>
		/// <item><description>FX_VERTEXALPHA</description></item><item><description>32</description></item><item><description>EnableVertexAlpha</description></item>
		/// </list>
		/// <para>
		/// Flags can be added to combine two or more effects.
		/// </para>
		/// <para>
		/// For example, specifying a flag of 3 (1+2) will result in both
		/// FullBright and EnableVertexColor effects to be enabled for all
		/// the surfaces of the specified entity.
		/// </para>
		/// <para>
		/// See <c>BrushFX</c> and <c>PaintSurface</c> for details on controlling 
		/// FX on a surface by surface basis.
		/// </para>
		/// <para>
		/// FullBright - disables standard diffuse and specular lighting
		/// caclulations during rendering so surface appears at 100%
		/// brightness.
		/// </para>
		/// <para>
		/// EnableVertexColors - vertex color information is used
		/// instead of surface colors when using vertex lighting 
		/// techniques.
		/// </para>
		/// <para>
		/// FlatShaded - uses lighting information from first vertex of
		/// each triangle instead of interpolating between all three as
		/// per default smooth shading.
		/// </para>
		/// <para>
		/// DisableFog - disables fogging calulations.
		/// </para>
		/// <para>
		/// DoubleSided- disables the back face culling method making
		/// both front facing and back facing sides of a surfaces 
		/// visible.
		/// </para>
		/// <para>
		/// EnableVertexAlpha - ensures the surface is treated as transparent
		/// by the Blitz3D rendering pipeline.
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		/// <param name="fx">fx flags</param>
		public static int EntityFX(int entity, int fx) { return bbEntityFX(entity, fx); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbPaintEntity(int entity, int brush);
		/// <summary>
		/// <para>
		/// Paints an entity with a brush.
		/// </para>
		/// <para>
		/// <c>PaintEntity</c> applies all the surface rendering properties 
		/// including color, alpha, texture, fx and blending modes
		/// to the entity specified.
		/// </para>
		/// <para>
		/// See the <c>CreateBrush</c> function for details about creating 
		/// a brush.
		/// </para>
		/// <para>
		/// Use the <c>PaintMesh</c> command to set all the surface properties
		/// of a mesh entity and <c>PaintSurface</c> for modifying rendering
		/// attributes on a particular surface.
		/// </para>
		/// <para>
		/// See the <c>EntityColor</c> command for information about how
		/// entity and surface properties are combined by the Blitz3D
		/// rendering pipeline.
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		/// <param name="brush">brush handle</param>
		public static int PaintEntity(int entity, int brush) { return bbPaintEntity(entity, brush); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbCollisions(int src_type, int dest_type, int detectionmethod, int response);
		/// <summary>
		/// <para>
		/// Configures method and response for collisions between two
		/// entities of the specified collision types.
		/// </para>
		/// <para>
		/// The <c>method</c> parameter can be one of the following:
		/// </para>
		/// <list type="table">
		/// <listheader><term>Method</term><term>Value</term><term>Description</term></listheader>
		/// <item><description>COLLIDE_SPHERESPHERE</description></item><item><description>1</description></item><item><description>Ellipsoid-to-ellipsoid collisions</description></item>
		/// <item><description>COLLIDE_SPHEREPOLY</description></item><item><description>2</description></item><item><description>Ellipsoid-to-polygon collisions</description></item>
		/// <item><description>COLLIDE_SPHEREBOX</description></item><item><description>3</description></item><item><description>Ellipsoid-to-box collisions</description></item>
		/// </list>
		/// <para>
		/// The <c>response</c> parameter can be one of the following:
		/// </para>
		/// <list type="table">
		/// <listheader><term>Response</term><term>Value</term><term>Description</term></listheader>
		/// <item><description>COLLIDE_STOP</description></item><item><description>1</description></item><item><description>source entity halts at point of collision</description></item>
		/// <item><description>COLLIDE_SLIDE1</description></item><item><description>2</description></item><item><description>slide source entity along the collision plane</description></item>
		/// <item><description>COLLIDE_SLIDE2</description></item><item><description>3</description></item><item><description>same as slide1 but y component ignored</description></item>
		/// </list>
		/// <para>
		/// After calling <c>UpdateWorld</c> the <c>CountCollisions</c>
		/// command can be used to detect collisions incurred
		/// by each entity and information about each of those
		/// collisions is returned by functions such as
		/// <c>EntityCollided,</c> <c>CollisionX,</c> <c>CollisionNX</c> etc.
		/// </para>
		/// <para>
		/// A series of calls to the <c>Collisions</c> command is usually
		/// only required during a game's initialization and not
		/// every game loop as <c>Collisions</c> settings remain effective 
		/// until a call to <c>ClearCollisions</c> or a call to <c>Collisions</c>
		/// with matching <c>source</c> and <c>target</c> entity types overwrites
		/// the existing method and reponse settings.
		/// </para>
		/// </summary>
		/// <param name="src_type">entity type to be checked for collisions.</param>
		/// <param name="dest_type">entity type to be collided with.</param>
		/// <param name="detectionmethod">collision detection method.</param>
		/// <param name="response">what the source entity does when a collision occurs.</param>
		public static int Collisions(int src_type, int dest_type, int detectionmethod, int response) { return bbCollisions(src_type, dest_type, detectionmethod, response); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbClearCollisions();
		/// <summary>
		/// <para>
		/// Clears the internal collision behavior table.
		/// </para>
		/// <para>
		/// Whenever a <c>Collisions</c> command is used to enable collisions 
		/// between two different entity types, an entry is added to
		/// an internal collision behavior table used by the <c>UpdateWorld</c>
		/// command.
		/// </para>
		/// <para>
		/// <c>ClearCollisions</c> clears the internal collision behavior
		/// table and has no affect on current entity collision state.
		/// </para>
		/// </summary>
		public static int ClearCollisions() { return bbClearCollisions(); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbEntityType(int entity, int collision_type, int recursive);
		/// <summary>
		/// <para>
		/// Sets the collision type for an entity.
		/// </para>
		/// <para>
		/// A collision_type value of 0 indicates that no collision checking will 
		/// occur with that entity.
		/// </para>
		/// <para>
		/// A collision value of 1-999 enables collision checking for the specified 
		/// entity and optionally all its children.
		/// </para>
		/// <para>
		/// The <c>UpdateWorld</c> command uses the currently active <c>Collisions</c> rules
		/// to perform various collision responses for the overlapping of entities
		/// that have a corresponding <c>EntityType.</c>
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		/// <param name="collision_type">collision type of entity. Must be in the range 0-999.</param>
		/// <param name="recursive">true to apply collision type to entity's children. Defaults to false.</param>
		public static int EntityType(int entity, int collision_type, int recursive) { return bbEntityType(entity, collision_type, recursive); }
		public static int EntityType(int entity, int collision_type) { return bbEntityType(entity, collision_type, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbGetEntityType(int entity);
		/// <summary>
		/// <para>
		/// Returns the collision type of an entity as set by the EntityType
		/// command.
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		public static int GetEntityType(int entity) { return bbGetEntityType(entity); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbResetEntity(int entity);
		/// <summary>
		/// <para>
		/// Resets the collision state of an entity.
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		public static int ResetEntity(int entity) { return bbResetEntity(entity); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbEntityRadius(int entity, float x_radius, float y_radius);
		/// <summary>
		/// <para>
		/// Sets the radius of an entity's collision sphere or if a 
		/// <c>y_radius</c> is specified the dimenstions of a collision
		/// ellipsoid.
		/// </para>
		/// <para>
		/// An entity radius should be set for all entities
		/// involved in ellipsoidal collisions.
		/// </para>
		/// <para>
		/// All entity types used as source entities in the 
		/// <c>Collisions</c> table (as collisions are always 
		/// ellipsoid-to-something), and any destination entity
		/// types specified in method 1 type <c>Collisions</c>
		/// entries (ellipsoid-to-ellipsoid collisions)
		/// require an <c>EntityRadius.</c>
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		/// <param name="x_radius">x radius of entity's collision ellipsoid</param>
		/// <param name="y_radius">y radius of entity's collision ellipsoid</param>
		public static int EntityRadius(int entity, float x_radius, float y_radius) { return bbEntityRadius(entity, x_radius, y_radius); }
		public static int EntityRadius(int entity, float x_radius) { return bbEntityRadius(entity, x_radius, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbEntityBox(int entity, float x, float y, float z, float width, float height, float depth);
		/// <summary>
		/// <para>
		/// Sets the dimensions of an entity's collision box.
		/// </para>
		/// <para>
		/// Any entity types featured as the destination of 
		/// type 3 <c>Collisions</c> (ellipsoid to box) require 
		/// an <c>EntityBox</c> to define their collision space.
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		/// <param name="x">x position of entity's collision box</param>
		/// <param name="y">y position of entity's collision box</param>
		/// <param name="z">z position of entity's collision box</param>
		/// <param name="width">width of entity's collision box</param>
		/// <param name="height">height of entity's collision box</param>
		/// <param name="depth">depth of entity's collision box</param>
		public static int EntityBox(int entity, float x, float y, float z, float width, float height, float depth) { return bbEntityBox(entity, x, y, z, width, height, depth); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbEntityCollided(int entity, int src_type);
		/// <summary>
		/// <para>
		/// Returns the handle of the entity of the specified type 
		/// that collided with the specified entity.
		/// </para>
		/// <para>
		/// Usually the <c>CountCollisions</c> function is used after an
		/// <c>UpdateWorld</c> with each collision being processed individually
		/// with the collision specific <c>CollisionX,</c> <c>CollisionY,</c> <c>CollisionZ,</c>
		/// <c>CollisionNX,</c> <c>CollisionNY,</c> <c>CollisionNZ,</c> <c>CountCollisions,</c>
		/// <c>EntityCollided,</c> <c>CollisionTime,</c> <c>CollisionEntity,</c> <c>CollisionSurface</c>
		/// and <c>CollisionTriangle</c> functions.
		/// </para>
		/// <para>
		/// <c>EntityCollided</c> provides a simple alternative in situations
		/// where a simple True or False collision result is required
		/// in regards to the specified entity type.
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		/// <param name="src_type">type of entity</param>
		public static int EntityCollided(int entity, int src_type) { return bbEntityCollided(entity, src_type); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbCountCollisions(int entity);
		/// <summary>
		/// <para>
		/// Returns how many collisions an entity was involved in during
		/// the last UpdateWorld.
		/// </para>
		/// <para>
		/// The <c>CountCollisions</c> function returns the maximum index value
		/// that should be used when fetching collision specific data 
		/// such as returned by the <c>CollisionX,</c> <c>CollisionY,</c> <c>CollisionZ,</c>
		/// <c>CollisionNX,</c> <c>CollisionNY,</c> <c>CollisionNZ,</c> <c>CountCollisions,</c>
		/// <c>EntityCollided,</c> <c>CollisionTime,</c> <c>CollisionEntity,</c> <c>CollisionSurface</c>
		/// and <c>CollisionTriangle</c> functions.
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		public static int CountCollisions(int entity) { return bbCountCollisions(entity); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbCollisionX(int entity, int index);
		/// <summary>
		/// <para>
		/// Returns the world x coordinate of a particular collision.
		/// </para>
		/// <para>
		/// Index should be in the range 1...CountCollisions( entity ) inclusive.
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		/// <param name="index">index of collision</param>
		public static float CollisionX(int entity, int index) { return bbCollisionX(entity, index); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbCollisionY(int entity, int index);
		/// <summary>
		/// <para>
		/// Returns the world y coordinate of a particular collision.
		/// </para>
		/// <para>
		/// Index should be in the range 1...CountCollisions( entity ) inclusive.
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		/// <param name="index">index of collision</param>
		public static float CollisionY(int entity, int index) { return bbCollisionY(entity, index); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbCollisionZ(int entity, int index);
		/// <summary>
		/// <para>
		/// Returns the world z coordinate of a particular collision.
		/// </para>
		/// <para>
		/// Index should be in the range 1...CountCollisions( entity ) inclusive.
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		/// <param name="index">index of collision</param>
		public static float CollisionZ(int entity, int index) { return bbCollisionZ(entity, index); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbCollisionNX(int entity, int index);
		/// <summary>
		/// <para>
		/// Returns the x component of the normal of a particular collision.
		/// </para>
		/// <para>
		/// Index should be in the range 1...CountCollisions( entity ) inclusive.
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		/// <param name="index">index of collision</param>
		public static float CollisionNX(int entity, int index) { return bbCollisionNX(entity, index); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbCollisionNY(int entity, int index);
		/// <summary>
		/// <para>
		/// Returns the y component of the normal of a particular collision.
		/// </para>
		/// <para>
		/// Index should be in the range 1...CountCollisions( entity ) inclusive.
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		/// <param name="index">index of collision</param>
		public static float CollisionNY(int entity, int index) { return bbCollisionNY(entity, index); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbCollisionNZ(int entity, int index);
		/// <summary>
		/// <para>
		/// Returns the z component of the normal of a particular collision.
		/// </para>
		/// <para>
		/// Index should be in the range 1...CountCollisions( entity ) inclusive.
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		/// <param name="index">index of collision</param>
		public static float CollisionNZ(int entity, int index) { return bbCollisionNZ(entity, index); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbCollisionTime(int entity, int index);
		/// <summary>
		/// <para>
		/// Returns the time at which the specified collision occured. A time of 0.0 means
		/// the collision ocurred at the beginning of the update period, a time of 1.0 means
		/// the collision ocurred at the end of the period. If the collision ocurred half way
		/// between the entities old position and its new a time of 0.5 will be returned.
		/// </para>
		/// <para>
		/// Index should be in the range 1...CountCollisions( entity ) inclusive representing 
		/// which collision from the most previous UpdateWorld.
		/// </para>
		/// <para>
		/// See the <c>UpdateWorld</c> command for more information on working with Blitz3D
		/// collisions.
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		/// <param name="index">index of collision</param>
		public static float CollisionTime(int entity, int index) { return bbCollisionTime(entity, index); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbCollisionEntity(int entity, int index);
		/// <summary>
		/// <para>
		/// Returns the other entity involved in a particular collision.
		/// </para>
		/// <para>
		/// Index should be in the range 1...CountCollisions( entity ), inclusive.
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		/// <param name="index">index of collision</param>
		public static int CollisionEntity(int entity, int index) { return bbCollisionEntity(entity, index); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbCollisionSurface(int entity, int index);
		/// <summary>
		/// <para>
		/// Returns the handle of the surface belonging to the specified entity that 
		/// was closest to the point of a particular collision.
		/// </para>
		/// <para>
		/// Index should be in the range 1...CountCollisions( entity ), inclusive.
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		/// <param name="index">index of collision</param>
		public static int CollisionSurface(int entity, int index) { return bbCollisionSurface(entity, index); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbCollisionTriangle(int entity, int index);
		/// <summary>
		/// <para>
		/// Returns the index number of the triangle belonging to the specified entity
		/// that was closest to the point of a particular collision.
		/// </para>
		/// <para>
		/// Index should be in the range 1...CountCollisions( entity ), inclusive.
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		/// <param name="index">index of collision</param>
		public static int CollisionTriangle(int entity, int index) { return bbCollisionTriangle(entity, index); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbEntityPickMode(int entity, int pick_geometry, int obscurer);
		/// <summary>
		/// <para>
		/// Sets the pick mode for an entity.
		/// </para>
		/// <para>
		/// The obscurer option if <c>True</c> specifies the entity 'obscures' other 
		/// entities during an EntityVisible call. Defaults to True.
		/// </para>
		/// <list type="table">
		/// <listheader><term>Mode</term><term>Value</term><term>Description</term></listheader>
		/// <item><description>PICK_NONE</description></item><item><description>0</description></item><item><description>Unpickable (default)</description></item>
		/// <item><description>PICK_SPHERE</description></item><item><description>1</description></item><item><description>Sphere (EntityRadius is used)</description></item>
		/// <item><description>PICK_POLY</description></item><item><description>2</description></item><item><description>Polygon</description></item>
		/// <item><description>PICK_BOX</description></item><item><description>3</description></item><item><description>Box (EntityBox is used)</description></item>
		/// </list>
		/// <para>
		/// The optional <c>obscurer</c> parameter is used with EntityVisible to determine 
		/// just what can get in the way of the line-of-sight between 2 entities.
		/// </para>
		/// <para>
		/// This allows some entities to be pickable using the other pick commands, 
		/// but to be ignored (i.e. 'transparent') when using EntityVisible.
		/// </para>
		/// <para>
		/// A valid mesh entity is required for Polygon type picking.
		/// Use Sphere or Box type picking for all other entity classes
		/// including sprites, terrains, pivots etc.
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		/// <param name="pick_geometry">type of geometry used for picking</param>
		/// <param name="obscurer">False to make entity transparent</param>
		public static int EntityPickMode(int entity, int pick_geometry, int obscurer) { return bbEntityPickMode(entity, pick_geometry, obscurer); }
		public static int EntityPickMode(int entity, int pick_geometry) { return bbEntityPickMode(entity, pick_geometry, 1); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbLinePick(float x, float y, float z, float dx, float dy, float dz, float radius);
		/// <summary>
		/// <para>
		/// Returns the first pickable entity along the line defined
		/// by the end coordinates (x,y,z) and (x+dx,y+dy,z+dz).
		/// </para>
		/// <para>
		/// Use the <c>EntityPickMode</c> command to make an entity
		/// pickable.
		/// </para>
		/// </summary>
		/// <param name="x">x coordinate of start of line pick</param>
		/// <param name="y">y coordinate of start of line pick</param>
		/// <param name="z">z coordinate of start of line pick</param>
		/// <param name="dx">distance x of line pick</param>
		/// <param name="dy">distance y of line pick</param>
		/// <param name="dz">distance z of line pick</param>
		/// <param name="radius">radius of line pick</param>
		public static int LinePick(float x, float y, float z, float dx, float dy, float dz, float radius) { return bbLinePick(x, y, z, dx, dy, dz, radius); }
		public static int LinePick(float x, float y, float z, float dx, float dy, float dz) { return bbLinePick(x, y, z, dx, dy, dz, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbEntityPick(int entity, float range);
		/// <summary>
		/// <para>
		/// Returns the nearest pickable entity 'infront' of the 
		/// specified entity.
		/// </para>
		/// <para>
		/// The scale of the <c>range</c> parameter is affected by the 
		/// scale of the entity and affects the maximum distance
		/// of the pick.
		/// </para>
		/// <para>
		/// Use the <c>EntityPickMode</c> command to make an entity
		/// pickable.
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		/// <param name="range">range of pick area around entity</param>
		public static int EntityPick(int entity, float range) { return bbEntityPick(entity, range); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbCameraPick(int camera, float viewport_x, float viewport_y);
		/// <summary>
		/// <para>
		/// Returns the nearest pickable entity occupying the specified 
		/// viewport coordinates or 0 if none.
		/// </para>
		/// <para>
		/// Use the <c>EntityPickMode</c> command to make an entity
		/// pickable.
		/// </para>
		/// <para>
		/// The <c>CameraPick</c> function is a useful way for detecting
		/// the entity being drawn at the specified screen location
		/// in particular when that location is the point at
		/// <c>MouseX,<c>MouseY.</c></c>
		/// </para>
		/// </summary>
		/// <param name="camera">camera handle</param>
		/// <param name="viewport_x">2D viewport coordinate</param>
		/// <param name="viewport_y">2D viewport coordinate</param>
		public static int CameraPick(int camera, float viewport_x, float viewport_y) { return bbCameraPick(camera, viewport_x, viewport_y); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbPickedX();
		/// <summary>
		/// <para>
		/// Returns the world X coordinate at which the most recently 
		/// picked entity was picked.
		/// </para>
		/// <para>
		/// The coordinate ( <c>PickedX(),</c> <c>PickedY(),</c> <c>PickedZ()</c> ) is
		/// the exact point in world space at which the current 
		/// <c>PickedEntity()</c> was picked with either the <c>CameraPick,</c> 
		/// <c>EntityPick</c> or <c>LinePick</c> functions.
		/// </para>
		/// </summary>
		public static float PickedX() { return bbPickedX(); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbPickedY();
		/// <summary>
		/// <para>
		/// Returns the world Y coordinate at which the most recently 
		/// picked entity was picked.
		/// </para>
		/// <para>
		/// The coordinate ( <c>PickedX(),</c> <c>PickedY(),</c> <c>PickedZ()</c> ) is
		/// the exact point in world space at which the current 
		/// <c>PickedEntity()</c> was picked with either the <c>CameraPick,</c> 
		/// <c>EntityPick</c> or <c>LinePick</c> functions.
		/// </para>
		/// </summary>
		public static float PickedY() { return bbPickedY(); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbPickedZ();
		/// <summary>
		/// <para>
		/// Returns the world Z coordinate at which the most recently 
		/// picked entity was picked.
		/// </para>
		/// <para>
		/// The coordinate ( <c>PickedX(),</c> <c>PickedY(),</c> <c>PickedZ()</c> ) is
		/// the exact point in world space at which the current 
		/// <c>PickedEntity()</c> was picked with either the <c>CameraPick,</c> 
		/// <c>EntityPick</c> or <c>LinePick</c> functions.
		/// </para>
		/// </summary>
		public static float PickedZ() { return bbPickedZ(); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbPickedNX();
		/// <summary>
		/// <para>
		/// Returns the X component of the surface normal at the point
		/// which the most recently picked entity was picked.
		/// </para>
		/// </summary>
		public static float PickedNX() { return bbPickedNX(); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbPickedNY();
		/// <summary>
		/// <para>
		/// Returns the Y component of the surface normal at the point
		/// which the most recently picked entity was picked.
		/// </para>
		/// </summary>
		public static float PickedNY() { return bbPickedNY(); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbPickedNZ();
		/// <summary>
		/// <para>
		/// Returns the Z component of the surface normal at the point
		/// which the most recently picked entity was picked.
		/// </para>
		/// </summary>
		public static float PickedNZ() { return bbPickedNZ(); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbPickedTime();
		/// <summary>
		/// <para>
		/// Returns a value between 0.0 and 1.0 representing the distance along the 
		/// <c>LinePick</c> at which the most recently picked entity was picked.
		/// </para>
		/// </summary>
		public static float PickedTime() { return bbPickedTime(); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbPickedEntity();
		/// <summary>
		/// <para>
		/// Returns the entity 'picked' by the most recently executed
		/// Pick command.
		/// </para>
		/// <para>
		/// Returns 0 if no entity was picked.
		/// </para>
		/// </summary>
		public static int PickedEntity() { return bbPickedEntity(); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbPickedSurface();
		/// <summary>
		/// <para>
		/// Returns the handle of the surface that was 'picked' by the
		/// most recently executed Pick command.
		/// </para>
		/// </summary>
		public static int PickedSurface() { return bbPickedSurface(); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbPickedTriangle();
		/// <summary>
		/// <para>
		/// Returns the index number of the triangle that was 'picked'
		/// by the most recently executed Pick command.
		/// </para>
		/// </summary>
		public static int PickedTriangle() { return bbPickedTriangle(); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbLoadMesh(String filename, int parent);
		/// <summary>
		/// <para>
		/// Returns a new mesh entity loaded from a .X, .3DS or .B3D file.
		/// </para>
		/// <para>
		/// Any hierarchy and animation information is ignored.
		/// </para>
		/// <para>
		/// Use the <c>LoadAnimMesh</c> function to load both mesh, hierarchy 
		/// and animation information.
		/// </para>
		/// <para>
		/// The optional <c>parent</c> parameter attaches the new mesh
		/// to a parent entity. See the <c>EntityParent</c> command for 
		/// more details on the effects of entity parenting. 
		/// </para>
		/// <para>
		/// The .b3d file format is the native file format of Blitz3D
		/// and supports features such as multitexturing, weighted 
		/// bone skinning and hierachial animation.
		/// </para>
		/// <para>
		/// See the <c>Blitz3D</c> File Format chapter for more information.
		/// </para>
		/// </summary>
		/// <param name="filename">name of the file containing the model to load</param>
		/// <param name="parent">optional parent entity</param>
		public static int LoadMesh(String filename, int parent) { return bbLoadMesh(filename, parent); }
		public static int LoadMesh(String filename) { return bbLoadMesh(filename, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbLoadAnimMesh(String filename, int parent);
		/// <summary>
		/// <para>
		/// Returns the root of an entity hierachy loaded from the specified file.
		/// </para>
		/// <para>
		/// Unlike <c>LoadMesh,</c> <c>LoadAnimMesh</c> may result in the creation of many
		/// mesh and pivot entities depending on what it finds in the specified
		/// file.
		/// </para>
		/// <para>
		/// The optional <c>parent</c> parameter attaches the new entity hierachy 
		/// to a parent entity. See the <c>EntityParent</c> command for more 
		/// details on the effects of entity parenting. 
		/// </para>
		/// <para>
		/// See the <c>Animate</c> command for activating any animation that may
		/// be included in the file.
		/// </para>
		/// <para>
		/// Locate child entities within an entity hierarchy by using the 
		/// <c>FindChild()</c> and <c>GetChild</c> functions.
		/// </para>
		/// <para>
		/// See the <c>Blitz3D</c> File Format for more information on Blitz3D's
		/// native file format and its support for multitexturing, weighted 
		/// bone skinning and hierachial animation.
		/// </para>
		/// </summary>
		/// <param name="filename">name of the file containing the model to load.</param>
		/// <param name="parent">optional entity to act as parent to the loaded mesh.</param>
		public static int LoadAnimMesh(String filename, int parent) { return bbLoadAnimMesh(filename, parent); }
		public static int LoadAnimMesh(String filename) { return bbLoadAnimMesh(filename, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbLoaderMatrix(String file_extension, float xx, float xy, float xz, float yx, float yy, float yz, float zx, float zy, float zz);
		/// <summary>
		/// <para>
		/// Sets a transformation matrix to be applied to specified file
		/// types when loaded.
		/// </para>
		/// <para>
		/// When geometric models loaded from file with the <c>LoadMesh</c>
		/// and <c>LoadAnimMesh</c> functions have been created in a different 
		/// coordinate system a <c>LoaderMatrix</c> transformation can be 
		/// used to correct the geometry at the time of loading.
		/// </para>
		/// <para>
		/// By default, the following loader matrices are used: 
		/// </para>
		/// <para>
		/// LoaderMatrix &quot;x&quot;,1,0,0,0,1,0,0,0,1 ; no change in coord system
		/// </para>
		/// <para>
		/// LoaderMatrix &quot;3ds&quot;,1,0,0,0,0,1,0,1,0 ; swap y/z axis' 
		/// </para>
		/// <para>
		/// You can use LoaderMatrix to flip meshes/animations if necessary,
		/// eg: 
		/// </para>
		/// <para>
		/// LoaderMatrix &quot;x&quot;,-1,0,0,0,1,0,0,0,1 ; flip x-cords for &quot;.x&quot; files
		/// </para>
		/// <para>
		/// LoaderMatrix &quot;3ds&quot;,-1,0,0,0,0,-1,0,1,0 ; swap y/z, negate x/z for &quot;.3ds&quot; files 
		/// </para>
		/// </summary>
		/// <param name="file_extension">file extension of 3d file</param>
		/// <param name="xx">1,1 element of 3x3 matrix</param>
		/// <param name="xy">2,1 element of 3x3 matrix</param>
		/// <param name="xz">3,1 element of 3x3 matrix</param>
		/// <param name="yx">1,2 element of 3x3 matrix</param>
		/// <param name="yy">2,2 element of 3x3 matrix</param>
		/// <param name="yz">3,2 element of 3x3 matrix</param>
		/// <param name="zx">1,3 element of 3x3 matrix</param>
		/// <param name="zy">2,3 element of 3x3 matrix</param>
		/// <param name="zz">3,3 element of 3x3 matrix</param>
		public static int LoaderMatrix(String file_extension, float xx, float xy, float xz, float yx, float yy, float yz, float zx, float zy, float zz) { return bbLoaderMatrix(file_extension, xx, xy, xz, yx, yy, yz, zx, zy, zz); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbCopyMesh(int mesh, int parent);
		/// <summary>
		/// <para>
		/// Creates a copy of a mesh entity and returns the newly-created mesh's
		/// handle.
		/// </para>
		/// <para>
		/// The optional <c>parent</c> parameter attaches the new copy 
		/// to a parent entity. See the <c>EntityParent</c> command for more 
		/// details on the effects of entity parenting. 
		/// </para>
		/// <para>
		/// The difference between <c>CopyMesh</c> and <c>CopyEntity</c> is that 
		/// <c>CopyMesh</c> makes a copy of all the surfaces whereas 
		/// the result of a <c>CopyEntity</c> shares any surfaces with its
		/// template.
		/// </para>
		/// <para>
		/// A mesh copy can also be created using a combination of the
		/// <c>CreateMesh</c> and <c>AddMesh</c> commands.
		/// </para>
		/// </summary>
		/// <param name="mesh">handle of mesh to be copied</param>
		/// <param name="parent">handle of entity to be made parent of mesh</param>
		public static int CopyMesh(int mesh, int parent) { return bbCopyMesh(mesh, parent); }
		public static int CopyMesh(int mesh) { return bbCopyMesh(mesh, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbCreateMesh(int parent);
		/// <summary>
		/// <para>
		/// Creates a 'blank' mesh entity and returns its handle.
		/// </para>
		/// <para>
		/// When a mesh is first created it has no surfaces, vertices or 
		/// triangles associated with it.
		/// </para>
		/// <para>
		/// To add geometry to a mesh, use the <c>AddMesh</c> command to copy
		/// surfaces from other meshes or new surfaces can be added with the
		/// <c>CreateSurface()</c> function with vertices and triangles being
		/// added to that surface using the <c>AddVertex</c> and <c>AddTriangle</c> 
		/// commands.
		/// </para>
		/// <para>
		/// The optional <c>parent</c> parameter attaches the new mesh
		/// to a parent entity. See the <c>EntityParent</c> command for 
		/// more details on the effects of entity parenting. 
		/// </para>
		/// </summary>
		/// <param name="parent">optional parent entity for new mesh</param>
		public static int CreateMesh(int parent) { return bbCreateMesh(parent); }
		public static int CreateMesh() { return bbCreateMesh(0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbAddMesh(int source_mesh, int dest_mesh);
		/// <summary>
		/// <para>
		/// Adds copies of all source_mesh's surfaces to the dest_mesh entities 
		/// surface list.
		/// </para>
		/// </summary>
		/// <param name="source_mesh">source mesh handle</param>
		/// <param name="dest_mesh">destination mesh handle</param>
		public static int AddMesh(int source_mesh, int dest_mesh) { return bbAddMesh(source_mesh, dest_mesh); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbFlipMesh(int mesh);
		/// <summary>
		/// <para>
		/// Flips all the triangles in a mesh.
		/// </para>
		/// <para>
		/// <c>FlipMesh</c> reverses the order of vertices for each triangle effectively
		/// making it face the opposite direction.
		/// </para>
		/// <para>
		/// Triangles that all face the wrong way is a common error when loading
		/// external meshes and the <c>FlipMesh</c> command is a useful correction
		/// if an alternative <c>LoaderMatrix</c> solution can not be found.
		/// </para>
		/// <para>
		/// <c>FlipMesh</c> is also useful for turning primitives created by <c>CreateSphere,</c>
		/// <c>CreateCylinder</c> and <c>CreateCone</c> inside out.
		/// </para>
		/// <para>
		/// See the <c>EntityFX</c> command for treating the triangles of a mesh as double
		/// sided instead.
		/// </para>
		/// </summary>
		/// <param name="mesh">mesh handle</param>
		public static int FlipMesh(int mesh) { return bbFlipMesh(mesh); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbPaintMesh(int mesh, int brush);
		/// <summary>
		/// <para>
		/// Paints a mesh with a brush.
		/// </para>
		/// <para>
		/// Color, texture, shininess, fx and blend mode properties are copied from
		/// the brush to each of the entity's surfaces.
		/// </para>
		/// <para>
		/// Use the <c>PaintSurface</c> command to paint individual surfaces.
		/// </para>
		/// <para>
		/// See the <c>CreateBrush()</c> function for more information about setting up a
		/// brush with which to paint entities and individual surfaces.
		/// </para>
		/// </summary>
		/// <param name="mesh">mesh handle</param>
		/// <param name="brush">brush handle</param>
		public static int PaintMesh(int mesh, int brush) { return bbPaintMesh(mesh, brush); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbLightMesh(int mesh, float red, float green, float blue, float range, float light_x, float light_y, float light_z);
		/// <summary>
		/// <para>
		/// Adds the effect of a specified point light to the color of all vertices.
		/// </para>
		/// <para>
		/// If the range parameter is omitted it is assumed to be 0 and all vertices
		/// are lit equally. If the optional position parameters are omitted the
		/// light is assumed to be at the local origin (coordinate 0,0,0).
		/// </para>
		/// <para>
		/// See the <c>EntityFX</c> command for selecting EnableVertexColors which must
		/// be active for the results of LightMesh to be visible.
		/// </para>
		/// <para>
		/// Because <c>LightMesh</c> is an additive operation and vertex colors default
		/// to white, negative white can be applied initially to reset all vertex
		/// colors to black:
		/// </para>
		/// <para>
		/// </para>
		/// </summary>
		/// <param name="mesh">mesh handle</param>
		/// <param name="red">mesh red value</param>
		/// <param name="green">mesh green value</param>
		/// <param name="blue">mesh blue value</param>
		/// <param name="range">optional light range</param>
		/// <param name="light_x">optional light x position</param>
		/// <param name="light_y">optional light y position</param>
		/// <param name="light_z">optional light z position</param>
		public static int LightMesh(int mesh, float red, float green, float blue, float range, float light_x, float light_y, float light_z) { return bbLightMesh(mesh, red, green, blue, range, light_x, light_y, light_z); }
		public static int LightMesh(int mesh, float red, float green, float blue, float range, float light_x, float light_y) { return bbLightMesh(mesh, red, green, blue, range, light_x, light_y, 0); }
		public static int LightMesh(int mesh, float red, float green, float blue, float range, float light_x) { return bbLightMesh(mesh, red, green, blue, range, light_x, 0, 0); }
		public static int LightMesh(int mesh, float red, float green, float blue, float range) { return bbLightMesh(mesh, red, green, blue, range, 0, 0, 0); }
		public static int LightMesh(int mesh, float red, float green, float blue) { return bbLightMesh(mesh, red, green, blue, 0, 0, 0, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbFitMesh(int mesh, float x, float y, float z, float width, float height, float depth, int uniform);
		/// <summary>
		/// <para>
		/// Scales and translates all vertices of a mesh so that the mesh 
		/// occupies the specified box.
		/// </para>
		/// <para>
		/// The <c>uniform</c> parameter defaults to false.
		/// </para>
		/// <para>
		/// A <c>uniform</c> fit will scale the size of the mesh evenly in
		/// each axis until the mesh fits in the dimensions specified
		/// retaining the mesh's original aspect ratio.
		/// </para>
		/// <para>
		/// A width, height or depth of 0 should never be used
		/// if the geometry of the mesh is to remain intact, use
		/// a value near 0 instead to &quot;flatten&quot; a mesh.
		/// </para>
		/// </summary>
		/// <param name="mesh">mesh handle</param>
		/// <param name="x">x position of mesh</param>
		/// <param name="y">y position ofmesh</param>
		/// <param name="z">z position of mesh</param>
		/// <param name="width">width of mesh</param>
		/// <param name="height">height of mesh</param>
		/// <param name="depth">depth of mesh</param>
		/// <param name="uniform">optional, True to scale all axis the same amount</param>
		public static int FitMesh(int mesh, float x, float y, float z, float width, float height, float depth, int uniform) { return bbFitMesh(mesh, x, y, z, width, height, depth, uniform); }
		public static int FitMesh(int mesh, float x, float y, float z, float width, float height, float depth) { return bbFitMesh(mesh, x, y, z, width, height, depth, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbScaleMesh(int mesh, float x_scale, float y_scale, float z_scale);
		/// <summary>
		/// <para>
		/// Scales all vertices of a mesh by the specified scaling factors.
		/// </para>
		/// </summary>
		/// <param name="mesh">mesh handle</param>
		/// <param name="x_scale">x scale of mesh</param>
		/// <param name="y_scale">y scale of mesh</param>
		/// <param name="z_scale">z scale of mesh</param>
		public static int ScaleMesh(int mesh, float x_scale, float y_scale, float z_scale) { return bbScaleMesh(mesh, x_scale, y_scale, z_scale); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbRotateMesh(int mesh, float pitch, float yaw, float roll);
		/// <summary>
		/// <para>
		/// Rotates all vertices of a mesh by the specified rotation.
		/// </para>
		/// <para>
		/// Rotation is in degrees where 360` is a complete rotation
		/// and the axis of each rotation is as follows:
		/// </para>
		/// <list type="table">
		/// <listheader><term>Name</term><term>Rotation Axis</term><term>Description</term></listheader>
		/// <item><description>Pitch</description></item><item><description>around x axis</description></item><item><description>equivalent to tilting forward/backwards.</description></item>
		/// <item><description>Yaw</description></item><item><description>around y axis</description></item><item><description> equivalent to turning left/right.</description></item>
		/// <item><description>Roll</description></item><item><description>around z axis</description></item><item><description> equivalent to tilting left/right.</description></item>
		/// </list>
		/// </summary>
		/// <param name="mesh">mesh handle</param>
		/// <param name="pitch">pitch of mesh</param>
		/// <param name="yaw">yaw of mesh</param>
		/// <param name="roll">roll of mesh</param>
		public static int RotateMesh(int mesh, float pitch, float yaw, float roll) { return bbRotateMesh(mesh, pitch, yaw, roll); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbPositionMesh(int mesh, float x, float y, float z);
		/// <summary>
		/// <para>
		/// Translates the position of all mesh vertices using the 
		/// specified direction vector.
		/// </para>
		/// </summary>
		/// <param name="mesh">mesh handle</param>
		/// <param name="x">x direction</param>
		/// <param name="y">y direction</param>
		/// <param name="z">z direction</param>
		public static int PositionMesh(int mesh, float x, float y, float z) { return bbPositionMesh(mesh, x, y, z); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbUpdateNormals(int mesh);
		/// <summary>
		/// <para>
		/// Recalculates all normals in a mesh.
		/// </para>
		/// <para>
		/// This is necessary for correct lighting if you are building
		/// or modifying meshes and have not set surface normals 
		/// manually using the <c>VertexNormal</c> commands
		/// or a mesh has been loaded from a model file with
		/// bad or missing vertex normal data.
		/// </para>
		/// </summary>
		/// <param name="mesh">mesh handle</param>
		public static int UpdateNormals(int mesh) { return bbUpdateNormals(mesh); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbMeshesIntersect(int mesh_a, int mesh_b);
		/// <summary>
		/// <para>
		/// Returns true if the specified meshes are currently
		/// intersecting.
		/// </para>
		/// <para>
		/// This is a fairly slow routine - use with discretion...
		/// </para>
		/// <para>
		/// This command is currently the only polygon-&gt;polygon collision checking
		/// routine available in Blitz3D.
		/// </para>
		/// </summary>
		/// <param name="mesh_a">mesh_a handle</param>
		/// <param name="mesh_b">mesh_b handle</param>
		public static int MeshesIntersect(int mesh_a, int mesh_b) { return bbMeshesIntersect(mesh_a, mesh_b); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbMeshWidth(int mesh);
		/// <summary>
		/// <para>
		/// Returns the width of a mesh. This is calculated by the actual vertex
		/// positions and so the scale of the entity (set by ScaleEntity) will not have
		/// an effect on the resultant width. Mesh operations, on the other hand, will
		/// effect the result.
		/// </para>
		/// </summary>
		/// <param name="mesh">mesh handle</param>
		public static float MeshWidth(int mesh) { return bbMeshWidth(mesh); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbMeshHeight(int mesh);
		/// <summary>
		/// <para>
		/// Returns the height of a mesh.
		/// This is calculated by the actual vertex positions and so the scale of the
		/// entity (set by ScaleEntity) will not have an effect on the resultant height.
		/// Mesh operations, on the other hand, will effect the result.
		/// </para>
		/// </summary>
		/// <param name="mesh">mesh handle</param>
		public static float MeshHeight(int mesh) { return bbMeshHeight(mesh); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbMeshDepth(int mesh);
		/// <summary>
		/// <para>
		/// Returns the depth of a mesh. This is calculated by the actual vertex positions
		/// and so the scale of the entity (set by ScaleEntity) will not have an effect
		/// on the resultant depth. Mesh operations, on the other hand, will effect
		/// the result.
		/// </para>
		/// </summary>
		/// <param name="mesh">mesh handle</param>
		public static float MeshDepth(int mesh) { return bbMeshDepth(mesh); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbMeshCullBox(int mesh, float x, float y, float z, float w, float h, float d);
		/// <summary>
		/// <para>
		/// This command allows the adjustment of the culling box
		/// used by the Blitz3D renderer when deciding if the
		/// mesh is outside the view of a camera.
		/// </para>
		/// <para>
		/// The culling box of a mesh is automatically calculated, 
		/// however in some instances an animated mesh may 
		/// stretch beyond this region resulting in it visually 
		/// popping out of view incorrectly. The <c>MeshCullBox</c>
		/// command allows a meshed culling box to be manually
		/// adjusted to correct this problem.
		/// </para>
		/// </summary>
		/// <param name="mesh">mesh handle</param>
		/// <param name="x">x position of far bottom left corner of bounding box</param>
		/// <param name="y">y position of far bottom left corner of bounding box</param>
		/// <param name="z">z position of far bottom left corner of bounding box</param>
		/// <param name="w">width of bounding box</param>
		/// <param name="h">height of bounding box</param>
		/// <param name="d">depth of bounding box</param>
		public static int MeshCullBox(int mesh, float x, float y, float z, float w, float h, float d) { return bbMeshCullBox(mesh, x, y, z, w, h, d); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbCountSurfaces(int mesh);
		/// <summary>
		/// <para>
		/// Returns the number of surfaces in a mesh.
		/// </para>
		/// <para>
		/// Surfaces are sections of a mesh with their own rendering
		/// properties.
		/// </para>
		/// <para>
		/// A mesh may contain none, one or many such surfaces. The
		/// vertices and triangles of a mesh actually belong to a particular
		/// surface of that mesh in Blitz3d.
		/// </para>
		/// </summary>
		/// <param name="mesh">mesh handle</param>
		public static int CountSurfaces(int mesh) { return bbCountSurfaces(mesh); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbGetSurface(int mesh, int index);
		/// <summary>
		/// <para>
		/// Returns the handle of the surface attached to the specified mesh
		/// and with the specified index number.
		/// </para>
		/// <para>
		/// Index should be in the range 1...CountSurfaces( mesh ), inclusive. 
		/// </para>
		/// <para>
		/// You need to 'get a surface', i.e. get its handle, in order to be able to 
		/// then use that particular surface with other commands.
		/// </para>
		/// <para>
		/// Often mesh surfaces in loaded models are not equivalent to how they
		/// were built in a modelling program due to optimzations and possible extra 
		/// edge vertices added by the Blitz3D loader. See the <c>FindSurface</c>
		/// command for an alternative method of locating a particular surface
		/// based on the brush properties used to create it.
		/// </para>
		/// </summary>
		/// <param name="mesh">mesh handle</param>
		/// <param name="index">index of surface</param>
		public static int GetSurface(int mesh, int index) { return bbGetSurface(mesh, index); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbCreateSurface(int mesh, int brush);
		/// <summary>
		/// <para>
		/// Creates a surface attached to a mesh and returns the surface's handle.
		/// </para>
		/// <para>
		/// Surfaces are sections of mesh which are then used
		/// to attach triangles to. You must have at least one surface per mesh in
		/// order to create a visible mesh.
		/// </para>
		/// <para>
		/// Multiple surfaces can be used per mesh when color and texture properties 
		/// vary in different sections of the same mesh.
		/// </para>
		/// <para>
		/// Splitting a mesh up into lots of sections allows you to affect those sections
		/// individually, which can be a lot more useful than if all the surfaces are
		/// combined into just a single surface. Single surface meshes however often 
		/// have the advantage of being faster to render.
		/// </para>
		/// </summary>
		/// <param name="mesh">mesh handle</param>
		/// <param name="brush">optional brush handle</param>
		public static int CreateSurface(int mesh, int brush) { return bbCreateSurface(mesh, brush); }
		public static int CreateSurface(int mesh) { return bbCreateSurface(mesh, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbPaintSurface(int surface, int brush);
		/// <summary>
		/// <para>
		/// Paints a surface with a brush.
		/// </para>
		/// <para>
		/// This has the effect of instantly altering the visible appearance of that 
		/// particular surface, i.e. section of mesh, assuming the brush's properties 
		/// are different to what was applied to the surface before. 
		/// </para>
		/// <para>
		/// See the <c>PaintEntity</c> command for more information about how
		/// entity properties are combined with surface properties in the
		/// Blitz3D rendering pipeline.
		/// </para>
		/// </summary>
		/// <param name="surface">surface handle</param>
		/// <param name="brush">brush handle</param>
		public static int PaintSurface(int surface, int brush) { return bbPaintSurface(surface, brush); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbClearSurface(int surface, int clear_verts, int clear_triangles);
		/// <summary>
		/// <para>
		/// Removes all vertices and / or triangles from a surface.
		/// </para>
		/// <para>
		/// The two optional parameters <c>clear_verts</c> and <c>clear_triangles</c> default
		/// to <c>True.</c>
		/// </para>
		/// </summary>
		/// <param name="surface">surface handle</param>
		/// <param name="clear_verts"><c>True</c> to remove all vertices from the specified surface</param>
		/// <param name="clear_triangles"><c>True</c> to remove all triangles from the specified surface</param>
		public static int ClearSurface(int surface, int clear_verts, int clear_triangles) { return bbClearSurface(surface, clear_verts, clear_triangles); }
		public static int ClearSurface(int surface, int clear_verts) { return bbClearSurface(surface, clear_verts, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbFindSurface(int mesh, int brush);
		/// <summary>
		/// <para>
		/// Attempts to find a surface attached to the specified mesh and created with
		/// properties similar to those in the specified brush.
		/// </para>
		/// <para>
		/// Returns the surface handle if found or 0 if not.
		/// </para>
		/// </summary>
		/// <param name="mesh">mesh handle</param>
		/// <param name="brush">brush handle</param>
		public static int FindSurface(int mesh, int brush) { return bbFindSurface(mesh, brush); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbAddVertex(int surface, float x, float y, float z, float u, float v, float w);
		/// <summary>
		/// <para>
		/// Returns the index of a new vertex added to the specified surface.
		/// </para>
		/// <para>
		/// The <c>x,</c> <c>y</c> and <c>z</c> parameters are the geometric coordinates of the new 
		/// vertex, and <c>u,</c> <c>v,</c> and <c>w</c> are texture mapping coordinates.
		/// </para>
		/// <para>
		/// By creating three vertices on a specific surface, their three index values can 
		/// then be used with <c>AddTriangle</c> to create a simple triangle mesh.
		/// </para>
		/// <para>
		/// The same vertices can be used as the corner of multiple triangles which is
		/// useful when creating surfaces with smooth edges.
		/// </para>
		/// <para>
		/// Multiple vertices in the same position are often required when the
		/// two sides of a sharp edge have different surface normals or there is
		/// a seem in the texture coordinates. Such situations require unique 
		/// vertices per face such as the cube created with <c>CreateCube</c> which 
		/// has 24 vertices not 8 in its single surface.
		/// </para>
		/// <para>
		/// See the <c>VertexTexCoords</c> command for more details on the optional u,v,w
		/// texture coordinates. The <c>u,</c> <c>v</c> and <c>w</c> parameters, if specified, effect 
		/// both texture coordinate sets (0 and 1).
		/// </para>
		/// <para>
		/// When adding a vertex its default color is 255,255,255,255.
		/// </para>
		/// </summary>
		/// <param name="surface">surface handle</param>
		/// <param name="x">x coordinate of vertex</param>
		/// <param name="y">y coordinate of vertex</param>
		/// <param name="z">z coordinate of vertex</param>
		/// <param name="u">u texture coordinate of vertex</param>
		/// <param name="v">v texture coordinate of vertex</param>
		/// <param name="w">w texture coordinate of vertex</param>
		public static int AddVertex(int surface, float x, float y, float z, float u, float v, float w) { return bbAddVertex(surface, x, y, z, u, v, w); }
		public static int AddVertex(int surface, float x, float y, float z, float u, float v) { return bbAddVertex(surface, x, y, z, u, v, 1); }
		public static int AddVertex(int surface, float x, float y, float z, float u) { return bbAddVertex(surface, x, y, z, u, 0, 1); }
		public static int AddVertex(int surface, float x, float y, float z) { return bbAddVertex(surface, x, y, z, 0, 0, 1); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbAddTriangle(int surface, int v0, int v1, int v2);
		/// <summary>
		/// <para>
		/// Returns the index of a new triangle added to the specified surface.
		/// </para>
		/// <para>
		/// The three vertex indexes define the points in clockwise order of 
		/// a single sided triangle that is added to the surface specified.
		/// </para>
		/// <para>
		/// The <c>v0,</c> <c>v1</c> and <c>v2</c> parameters are the index numbers of vertices added
		/// to the same surface using the AddVertex function. 
		/// </para>
		/// <para>
		/// A special DoubleSided effect can be enabled for a surface that
		/// will treat each triangle in a surface as having two sides, see
		/// <c>EntityFX</c> and <c>BrushFX</c> for more information.
		/// </para>
		/// </summary>
		/// <param name="surface">surface handle</param>
		/// <param name="v0">index number of first vertex of triangle</param>
		/// <param name="v1">index number of second vertex of triangle</param>
		/// <param name="v2">index number of third vertex of triangle</param>
		public static int AddTriangle(int surface, int v0, int v1, int v2) { return bbAddTriangle(surface, v0, v1, v2); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbVertexCoords(int surface, int index, float x, float y, float z);
		/// <summary>
		/// <para>
		/// Sets the geometric coordinates of an existing vertex.
		/// </para>
		/// <para>
		/// The index value should be in the range 0..CountVertices()-1.
		/// </para>
		/// <para>
		/// By changing the position of individual verticies in a mesh, dynamic
		/// 'mesh deforming' effects and high performance 'particle systems'
		/// can be programmed in Blitz3D.
		/// </para>
		/// <para>
		/// See the <c>VertexNormal</c> or <c>UpdateNormals</c> commands for correcting
		/// lighting errors that may be introduced when deforming a mesh.
		/// </para>
		/// </summary>
		/// <param name="surface">surface handle</param>
		/// <param name="index">index of vertex</param>
		/// <param name="x">x position of vertex</param>
		/// <param name="y">y position of vertex</param>
		/// <param name="z">z position of vertex</param>
		public static int VertexCoords(int surface, int index, float x, float y, float z) { return bbVertexCoords(surface, index, x, y, z); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbVertexNormal(int surface, int index, float nx, float ny, float nz);
		/// <summary>
		/// <para>
		/// Sets the normal of an existing vertex.
		/// </para>
		/// <para>
		/// The index value should be in the range 0..CountVertices()-1.
		/// </para>
		/// <para>
		/// Depending on the suface properties and the type of active 
		/// lights in the world vertex normals can play a big part in 
		/// rendering correctly shaded surfaces.
		/// </para>
		/// <para>
		/// A vertex normal should point directly away from any
		/// triangle faces the vertex has been used to construct.
		/// </para>
		/// <para>
		/// See the <c>UpdateNormals</c> command to automatically
		/// calculate the surface normals of all vertices in a mesh.
		/// </para>
		/// </summary>
		/// <param name="surface">surface handle</param>
		/// <param name="index">index of vertex</param>
		/// <param name="nx">normal x of vertex</param>
		/// <param name="ny">normal y of vertex</param>
		/// <param name="nz">normal z of vertex</param>
		public static int VertexNormal(int surface, int index, float nx, float ny, float nz) { return bbVertexNormal(surface, index, nx, ny, nz); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbVertexColor(int surface, int index, float red, float green, float blue, float alpha);
		/// <summary>
		/// <para>
		/// Sets the color of an existing vertex.
		/// </para>
		/// <para>
		/// Red, green and blue paramaters should all be in the range 0..255.
		/// See the <c>Color</c> command for more information on combining
		/// red, green and blue values to define specific colors.
		/// </para>
		/// <para>
		/// The index value should be in the range 0..CountVertices()-1.
		/// </para>
		/// <para>
		/// If you want to set the alpha individually for vertices using the 
		/// <c>alpha</c> parameter then you need to use EntityFX flags: 32 (to force 
		/// alpha-blending) and 2 (to switch to vertex colors).
		/// </para>
		/// </summary>
		/// <param name="surface">surface handle</param>
		/// <param name="index">index of vertex</param>
		/// <param name="red">red value of vertex</param>
		/// <param name="green">green value of vertex</param>
		/// <param name="blue">blue value of vertex</param>
		/// <param name="alpha">optional alpha transparency of vertex (0.0 to 1.0 - default: 1.0)</param>
		public static int VertexColor(int surface, int index, float red, float green, float blue, float alpha) { return bbVertexColor(surface, index, red, green, blue, alpha); }
		public static int VertexColor(int surface, int index, float red, float green, float blue) { return bbVertexColor(surface, index, red, green, blue, 1); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbVertexTexCoords(int surface, int index, float u, float v, float w, int coord_set);
		/// <summary>
		/// <para>
		/// Sets the texture coordinates of an existing vertex.
		/// </para>
		/// <para>
		/// The index value should be in the range 0..CountVertices()-1.
		/// </para>
		/// <para>
		/// Texture coordinates determine how any active texturing for a
		/// surface will be positioned on triangles by changing the
		/// texture location used at each vertex corner.
		/// </para>
		/// <para>
		/// This works on the following basis: 
		/// </para>
		/// <para>
		/// The top left of an image has the uv coordinates 0,0. 
		/// </para>
		/// <para>
		/// The top right has coordinates 1,0.
		/// </para>
		/// <para>
		/// The bottom right is 1,1.
		/// </para>
		/// <para>
		/// The bottom left 0,1. 
		/// </para>
		/// <para>
		/// Thus, uv coordinates for a vertex correspond to a point in the 
		/// image. For example, coordinates 0.9,0.1 would be near the upper 
		/// right corner of the image. The w parameter is currently ignored.
		/// </para>
		/// <para>
		/// The coord_set specifies which of two vertex texture coordinates 
		/// are to be modified. Secondary texture coordinates are sometimes
		/// useful when multitexturing with the <c>TextureCoords</c> controlling
		/// which texture coordinate set is used by each texture applied to
		/// the vertices' surface.
		/// </para>
		/// </summary>
		/// <param name="surface">surface handle</param>
		/// <param name="index">index of vertex</param>
		/// <param name="u">u coordinate of vertex</param>
		/// <param name="v">v coordinate of vertex</param>
		/// <param name="w">w coordinate of vertex</param>
		/// <param name="coord_set">co_oord set. Should be set to 0 or 1.</param>
		public static int VertexTexCoords(int surface, int index, float u, float v, float w, int coord_set) { return bbVertexTexCoords(surface, index, u, v, w, coord_set); }
		public static int VertexTexCoords(int surface, int index, float u, float v, float w) { return bbVertexTexCoords(surface, index, u, v, w, 0); }
		public static int VertexTexCoords(int surface, int index, float u, float v) { return bbVertexTexCoords(surface, index, u, v, 1, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbCountVertices(int surface);
		/// <summary>
		/// <para>
		/// Returns the number of vertices in a surface.
		/// </para>
		/// <para>
		/// Use the result of <c>CountVertices</c> command to make sure your
		/// program only modifies vertices that exist. Vertex modifier
		/// commands such as <c>VertexCoords,</c> <c>VertexColor,</c> <c>VertexNormal</c>
		/// and <c>VertexTexCoords</c> all use a vertex index parameter that should
		/// be in the range of 0..CountVertices()-1.
		/// </para>
		/// </summary>
		/// <param name="surface">surface handle</param>
		public static int CountVertices(int surface) { return bbCountVertices(surface); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbCountTriangles(int surface);
		/// <summary>
		/// <para>
		/// Returns the number of triangles in a surface.
		/// </para>
		/// </summary>
		/// <param name="surface">surface handle</param>
		public static int CountTriangles(int surface) { return bbCountTriangles(surface); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbVertexX(int surface, int index);
		/// <summary>
		/// <para>
		/// Returns the x coordinate of a vertex.
		/// </para>
		/// </summary>
		/// <param name="surface">surface handle</param>
		/// <param name="index">index of vertex</param>
		public static float VertexX(int surface, int index) { return bbVertexX(surface, index); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbVertexY(int surface, int index);
		/// <summary>
		/// <para>
		/// Returns the y coordinate of a vertex.
		/// </para>
		/// </summary>
		/// <param name="surface">surface handle</param>
		/// <param name="index">index of vertex</param>
		public static float VertexY(int surface, int index) { return bbVertexY(surface, index); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbVertexZ(int surface, int index);
		/// <summary>
		/// <para>
		/// Returns the z coordinate of a vertex.
		/// </para>
		/// </summary>
		/// <param name="surface">surface handle</param>
		/// <param name="index">index of vertex</param>
		public static float VertexZ(int surface, int index) { return bbVertexZ(surface, index); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbVertexNX(int surface, int index);
		/// <summary>
		/// <para>
		/// Returns the x component of a vertices normal.
		/// </para>
		/// </summary>
		/// <param name="surface">surface handle</param>
		/// <param name="index">index of vertex</param>
		public static float VertexNX(int surface, int index) { return bbVertexNX(surface, index); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbVertexNY(int surface, int index);
		/// <summary>
		/// <para>
		/// Returns the y component of a vertices normal.
		/// </para>
		/// </summary>
		/// <param name="surface">surface handle</param>
		/// <param name="index">index of vertex</param>
		public static float VertexNY(int surface, int index) { return bbVertexNY(surface, index); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbVertexNZ(int surface, int index);
		/// <summary>
		/// <para>
		/// Returns the z component of a vertices normal.
		/// </para>
		/// </summary>
		/// <param name="surface">surface handle</param>
		/// <param name="index">index of vertex</param>
		public static float VertexNZ(int surface, int index) { return bbVertexNZ(surface, index); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbVertexRed(int surface, int index);
		/// <summary>
		/// <para>
		/// Returns the red component of a vertices color.
		/// </para>
		/// </summary>
		/// <param name="surface">surface handle</param>
		/// <param name="index">index of vertex</param>
		public static float VertexRed(int surface, int index) { return bbVertexRed(surface, index); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbVertexGreen(int surface, int index);
		/// <summary>
		/// <para>
		/// Returns the green component of a vertices color.
		/// </para>
		/// </summary>
		/// <param name="surface">surface handle</param>
		/// <param name="index">index of vertex</param>
		public static float VertexGreen(int surface, int index) { return bbVertexGreen(surface, index); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbVertexBlue(int surface, int index);
		/// <summary>
		/// <para>
		/// Returns the blue component of a vertices color.
		/// </para>
		/// </summary>
		/// <param name="surface">surface handle</param>
		/// <param name="index">index of vertex</param>
		public static float VertexBlue(int surface, int index) { return bbVertexBlue(surface, index); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbVertexAlpha(int surface, int index);
		/// <summary>
		/// <para>
		/// Returns the alpha component of a vertices color, set using
		/// <c>VertexColor.</c>
		/// </para>
		/// </summary>
		/// <param name="surface">surface handle</param>
		/// <param name="index">index of vertex</param>
		public static float VertexAlpha(int surface, int index) { return bbVertexAlpha(surface, index); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbVertexU(int surface, int index, int coord_set);
		/// <summary>
		/// <para>
		/// Returns the texture u coordinate of a vertex.
		/// </para>
		/// <para>
		/// The coord_set defaults to 0 but may optionally be 1
		/// to specify the value returned refer to the vertex's 
		/// secondary texture coordinate set.
		/// </para>
		/// </summary>
		/// <param name="surface">surface handle</param>
		/// <param name="index">index of vertex</param>
		/// <param name="coord_set">optional UV mapping coordinate set</param>
		public static float VertexU(int surface, int index, int coord_set) { return bbVertexU(surface, index, coord_set); }
		public static float VertexU(int surface, int index) { return bbVertexU(surface, index, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbVertexV(int surface, int index, int coord_set);
		/// <summary>
		/// <para>
		/// Returns the texture v coordinate of a vertex.
		/// </para>
		/// <para>
		/// The coord_set defaults to 0 but may optionally be 1
		/// to specify the value returned refer to the vertex's 
		/// secondary texture coordinate set.
		/// </para>
		/// </summary>
		/// <param name="surface">surface handle</param>
		/// <param name="index">index of vertex</param>
		/// <param name="coord_set">optional UV mapping coordinate set. Should be set to 0 or 1.</param>
		public static float VertexV(int surface, int index, int coord_set) { return bbVertexV(surface, index, coord_set); }
		public static float VertexV(int surface, int index) { return bbVertexV(surface, index, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbVertexW(int surface, int index);
		/// <summary>
		/// <para>
		/// Returns the texture w coordinate of a vertex.
		/// </para>
		/// <para>
		/// The coord_set defaults to 0 but may optionally be 1
		/// to specify the value returned refer to the vertex's 
		/// secondary texture coordinate set.
		/// </para>
		/// </summary>
		/// <param name="surface">surface handle</param>
		/// <param name="index">index of vertex</param>
		public static float VertexW(int surface, int index) { return bbVertexW(surface, index); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbTriangleVertex(int surface, int triangle_index, int corner);
		/// <summary>
		/// <para>
		/// Returns the vertex index of a triangle corner.
		/// </para>
		/// </summary>
		/// <param name="surface">surface handle</param>
		/// <param name="triangle_index">triangle index</param>
		/// <param name="corner">corner of triangle. Should be 0, 1 or 2.</param>
		public static int TriangleVertex(int surface, int triangle_index, int corner) { return bbTriangleVertex(surface, triangle_index, corner); }

		[DllImport("Blitz3D.dll")]
		public static extern String bbEntityClass(int entity);
		/// <summary>
		/// <para>
		/// Returns a string containing the class of the specified entity.
		/// </para>
		/// <para>
		/// Possible return values are &quot;Pivot&quot;, &quot;Light&quot;,&quot;Camera&quot;, &quot;Mirror&quot;, &quot;Listener&quot;,
		/// &quot;Sprite&quot;, &quot;Terrain&quot;, &quot;Plane&quot;, &quot;Mesh&quot;, &quot;MD2&quot; and &quot;BSP&quot;.
		/// </para>
		/// <para>
		/// Note that <c>EntityClass</c> function will fail if a valid entity handle is not 
		/// supplied and will not just return an empty string.
		/// </para>
		/// </summary>
		/// <param name="entity">a valid entity handle</param>
		public static String EntityClass(int entity) { return bbEntityClass(entity); }

		[DllImport("Blitz3D.dll")]
		public static extern String bbEntityName(int entity);
		/// <summary>
		/// <para>
		/// Returns the name of an entity.
		/// </para>
		/// <para>
		/// An entity's name may be set when it was loaded from a model file
		/// or from the use of the <c>NameEntity</c> command.
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		public static String EntityName(int entity) { return bbEntityName(entity); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbNameEntity(int entity, String name);
		/// <summary>
		/// <para>
		/// Sets an entity's name.
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		/// <param name="name">name of entity</param>
		public static int NameEntity(int entity, String name) { return bbNameEntity(entity, name); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbEntityID(int entity);
		/// <summary>
		/// <para>
		/// Returns an integer identifier.
		/// </para>
		/// <para>
		/// An entity's id is zero unless one has been specified earlier using the
		/// <c>SetEntityID</c> command.
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		public static int EntityID(int entity) { return bbEntityID(entity); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbSetEntityID(int entity, int id);
		/// <summary>
		/// <para>
		/// Sets an entity's identifier field which can later be retrieved using the
		/// <c>EntityID</c> command.
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		/// <param name="id">any integer value</param>
		public static int SetEntityID(int entity, int id) { return bbSetEntityID(entity, id); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbCaptureEntity(int entity);
		/// <summary>
		/// <para>
		/// Takes a snapshot of the entity including the position,
		/// rotation, scale and alpha. 
		/// </para>
		/// <para>
		/// The <c>CaptureEntity</c> command can be used after
		/// <c>CaptureWorld</c> to control the tweening of a specific
		/// Entity. This is most useful when an entity needs to
		/// be repositioned and tweening between it's last state
		/// and current is not desired.
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		public static int CaptureEntity(int entity) { return bbCaptureEntity(entity); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbEntityVisible(int src_entity, int dest_entity);
		/// <summary>
		/// <para>
		/// Returns <c>True</c> if src_entity and dest_entity can 'see' each other.
		/// </para>
		/// <para>
		/// This command casts a ray (an imaginary line) from src_entity to 
		/// dest_entity. If the ray hits an obscurer entity the result is <c>False</c> 
		/// otherwise the function returns <c>True.</c>
		/// </para>
		/// <para>
		/// See the <c>EntityPickMode</c> for setting an entity as an obscurer.
		/// </para>
		/// </summary>
		/// <param name="src_entity">source entity handle</param>
		/// <param name="dest_entity">destination entity handle</param>
		public static int EntityVisible(int src_entity, int dest_entity) { return bbEntityVisible(src_entity, dest_entity); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbEntityDistance(int src_entity, int dest_entity);
		/// <summary>
		/// <para>
		/// Returns the distance between src_entity and dest_entity.
		/// </para>
		/// </summary>
		/// <param name="src_entity">source entity handle</param>
		/// <param name="dest_entity">destination entity handle</param>
		public static float EntityDistance(int src_entity, int dest_entity) { return bbEntityDistance(src_entity, dest_entity); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbDeltaYaw(int src_entity, int dest_entity);
		/// <summary>
		/// <para>
		/// Returns the yaw angle, that src_entity should be rotated by in order 
		/// to face dest_entity.
		/// </para>
		/// <para>
		/// This command can be used to be point one entity at another, rotating on
		/// the y axis only.
		/// </para>
		/// </summary>
		/// <param name="src_entity">source entity handle</param>
		/// <param name="dest_entity">destination entity handle</param>
		public static float DeltaYaw(int src_entity, int dest_entity) { return bbDeltaYaw(src_entity, dest_entity); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbDeltaPitch(int src_entity, int dest_entity);
		/// <summary>
		/// <para>
		/// Returns the pitch angle, that src_entity should be rotated by in order 
		/// to face dest_entity.
		/// </para>
		/// <para>
		/// This command can be used to be point one entity at another, rotating on
		/// the x axis only.
		/// </para>
		/// </summary>
		/// <param name="src_entity">source entity handle</param>
		/// <param name="dest_entity">destination entity handle</param>
		public static float DeltaPitch(int src_entity, int dest_entity) { return bbDeltaPitch(src_entity, dest_entity); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbAnimate(int entity, int mode, float speed, int sequence, float transition);
		/// <summary>
		/// <para>
		/// Animates an entity.
		/// </para>
		/// <para>
		/// The mode specified can be one of the following values:
		/// </para>
		/// <list type="table">
		/// <listheader><term>Mode</term><term>Value</term><term>Descriptiopn</term></listheader>
		/// <item><description>ANIM_STOP</description></item><item><description>0</description></item><item><description>Stop animation</description></item>
		/// <item><description>ANIM_LOOP</description></item><item><description>1</description></item><item><description>Loop animation (default)</description></item>
		/// <item><description>ANIM_PINGPONG</description></item><item><description>2</description></item><item><description>Ping-pong animation</description></item>
		/// <item><description>ANIM_ONCE</description></item><item><description>3</description></item><item><description>One-shot animation</description></item>
		/// </list>
		/// <para>
		/// A <c>speed</c> of greater than 1.0 will cause the animation to replay
		/// quicker, less than 1.0 slower. A negative speed will play the 
		/// animation backwards.
		/// </para>
		/// <para>
		/// Animation sequences are numbered 0,1,2...etc. Initially, an entity 
		/// loaded with <c>LoadAnimMesh</c> will have a single animation sequence.
		/// </para>
		/// <para>
		/// More sequences can be added using either <c>ExtractAnimSeq,</c> 
		/// <c>LoadAnimSeq</c> or <c>AddAnimSeq.</c>
		/// </para>
		/// <para>
		/// The optional <c>transition</c> parameter can be set to 0 to cause an instant 
		/// 'leap' to the first frame, while values greater than 0 will cause a 
		/// smoother transition to occur.
		/// </para>
		/// <para>
		/// While <c>Animate</c> begins or ends an animation calling <c>UpdateWorld</c> 
		/// once every main loop causes the animation to actually play.
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		/// <param name="mode">mode of animation. Defaults to 1.</param>
		/// <param name="speed">speed of animation. Defaults to 1.</param>
		/// <param name="sequence">specifies which sequence of animation frames to play. Defaults to 0.</param>
		/// <param name="transition">used to tween between an entity's current position rotation and the first frame of animation. Defaults to 0.</param>
		public static int Animate(int entity, int mode, float speed, int sequence, float transition) { return bbAnimate(entity, mode, speed, sequence, transition); }
		public static int Animate(int entity, int mode, float speed, int sequence) { return bbAnimate(entity, mode, speed, sequence, 0); }
		public static int Animate(int entity, int mode, float speed) { return bbAnimate(entity, mode, speed, 0, 0); }
		public static int Animate(int entity, int mode) { return bbAnimate(entity, mode, 1.0f, 0, 0); }
		public static int Animate(int entity) { return bbAnimate(entity, 1, 1.0f, 0, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbExtractAnimSeq(int entity, int first_frame, int last_frame, int anim_seq);
		/// <summary>
		/// <para>
		/// This command allows you to convert an animation with an MD2-style series of
		/// anim sequences into a pure Blitz anim sequence, and play it back as such
		/// using Animate.
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		/// <param name="first_frame">first frame of anim sequence to extract</param>
		/// <param name="last_frame">last frame of anim sequence to extract</param>
		/// <param name="anim_seq">anim sequence to extract from. This is usually 0, and as such defaults to 0.</param>
		public static int ExtractAnimSeq(int entity, int first_frame, int last_frame, int anim_seq) { return bbExtractAnimSeq(entity, first_frame, last_frame, anim_seq); }
		public static int ExtractAnimSeq(int entity, int first_frame, int last_frame) { return bbExtractAnimSeq(entity, first_frame, last_frame, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbAddAnimSeq(int entity, int length);
		/// <summary>
		/// <para>
		/// Creates an animation sequence for an entity.
		/// </para>
		/// <para>
		/// This must be done before any animation keys set by <c>SetAnimKey</c> can be 
		/// used in an actual animation however this is optional.
		/// </para>
		/// <para>
		/// You may use it to &quot;bake&quot; the frames you have added previously
		/// using SetAnimKey.
		/// </para>
		/// <para>
		/// Returns the animation sequence number added.
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		/// <param name="length">number of frames to be added</param>
		public static int AddAnimSeq(int entity, int length) { return bbAddAnimSeq(entity, length); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbSetAnimKey(int entity, int frame, int pos_key, int rot_key, int scale_key);
		/// <summary>
		/// <para>
		/// Sets an animation key for the specified entity at the specified frame.
		/// </para>
		/// <para>
		/// The entity must have a valid animation sequence to work with.
		/// </para>
		/// <para>
		/// This is most useful when you've got a character, or a complete set of 
		/// complicated moves to perform, and you want to perform them en-masse.
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		/// <param name="frame">frame of animation to be used as anim key</param>
		/// <param name="pos_key">true to include entity position information when setting key. Defaults to true.</param>
		/// <param name="rot_key">true to include entity rotation information when setting key. Defaults to true.</param>
		/// <param name="scale_key">true to include entity scale information when setting key. Defaults to true.</param>
		public static int SetAnimKey(int entity, int frame, int pos_key, int rot_key, int scale_key) { return bbSetAnimKey(entity, frame, pos_key, rot_key, scale_key); }
		public static int SetAnimKey(int entity, int frame, int pos_key, int rot_key) { return bbSetAnimKey(entity, frame, pos_key, rot_key, 1); }
		public static int SetAnimKey(int entity, int frame, int pos_key) { return bbSetAnimKey(entity, frame, pos_key, 1, 1); }
		public static int SetAnimKey(int entity, int frame) { return bbSetAnimKey(entity, frame, 1, 1, 1); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbLoadAnimSeq(int entity, String filename);
		/// <summary>
		/// <para>
		/// Appends an animation sequence from a file to an entity.
		/// </para>
		/// <para>
		/// Returns the animation sequence number added.
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		/// <param name="filename">filename of animated 3D object</param>
		public static int LoadAnimSeq(int entity, String filename) { return bbLoadAnimSeq(entity, filename); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbSetAnimTime(int entity, float time, int anim_seq);
		/// <summary>
		/// <para>
		/// SetAnimTime allows you to manually animate entities.
		/// </para>
		/// <para>
		/// A combination of <c>Animate</c> and <c>UpdateWorld</c> are usually
		/// required for animation however the <c>SetAnimTime</c> allows
		/// an alternative method for the program to control animation
		/// by manually controlling the entity's progress along its
		/// animation timeline.
		/// </para>
		/// </summary>
		/// <param name="entity">a valid entity handle.</param>
		/// <param name="time">a floating point time value.</param>
		/// <param name="anim_seq">an optional animation sequence number.</param>
		public static int SetAnimTime(int entity, float time, int anim_seq) { return bbSetAnimTime(entity, time, anim_seq); }
		public static int SetAnimTime(int entity, float time) { return bbSetAnimTime(entity, time, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbAnimSeq(int entity);
		/// <summary>
		/// <para>
		/// Returns the specified entity's current animation sequence.
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		public static int AnimSeq(int entity) { return bbAnimSeq(entity); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbAnimLength(int entity);
		/// <summary>
		/// <para>
		/// Returns the length or duration of the specified entity's current
		/// animation sequence. The value returned is equivalent to the
		/// number of frames or calls to UpdateWorld required to play
		/// the entire animation once.
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		public static int AnimLength(int entity) { return bbAnimLength(entity); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbAnimTime(int entity);
		/// <summary>
		/// <para>
		/// Returns the current animation time of an entity.
		/// </para>
		/// <para>
		/// <c>AnimTime</c> returns a float between 0.0 and the value returned
		/// by the <c>AnimLength()</c> function unless the animtion is in transition
		/// in which case a value of 0.0 is returned.
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		public static float AnimTime(int entity) { return bbAnimTime(entity); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbAnimating(int entity);
		/// <summary>
		/// <para>
		/// Returns true if the specified entity is currently animating.
		/// </para>
		/// </summary>
		/// <param name="entity">entity handle</param>
		public static int Animating(int entity) { return bbAnimating(entity); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbCreateTerrain(int grid_size, int parent);
		/// <summary>
		/// <para>
		/// Creates a terrain entity and returns its handle.
		/// </para>
		/// <para>
		/// The terrain extends from 0,0,0 to grid_size,1,grid_size. 
		/// </para>
		/// <para>
		/// The grid_size, no of grid squares along each side of terrain, and must 
		/// be a power of 2 value, e.g. 32, 64, 128, 256, 512, 1024.
		/// </para>
		/// <para>
		/// A terrain is a special type of polygon object that uses real-time level of
		/// detail (LOD) to display landscapes which should theoretically consist of 
		/// over a million polygons with only a few thousand.
		/// </para>
		/// <para>
		/// The way it does this is by constantly rearranging a certain amount of
		/// polygons to display high levels of detail close to the viewer and low
		/// levels further away. 
		/// </para>
		/// <para>
		/// This constant rearrangement of polygons is occasionally noticeable 
		/// however, and is a well-known side-effect of all LOD landscapes.
		/// </para>
		/// <para>
		/// This 'pop-in' effect can be reduced in lots of ways though, as the 
		/// other terrain help files will go on to explain. 
		/// </para>
		/// <para>
		/// The optional <c>parent</c> parameter attaches the new terrain
		/// to a parent entity. See the <c>EntityParent</c> command for 
		/// more details on the effects of entity parenting. 
		/// </para>
		/// </summary>
		/// <param name="grid_size">no of grid squares along each side of terrain</param>
		/// <param name="parent">optional parent entity of terrain</param>
		public static int CreateTerrain(int grid_size, int parent) { return bbCreateTerrain(grid_size, parent); }
		public static int CreateTerrain(int grid_size) { return bbCreateTerrain(grid_size, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbLoadTerrain(String file, int parent);
		/// <summary>
		/// <para>
		/// Loads a terrain from an image file and returns the terrain's handle.
		/// </para>
		/// <para>
		/// The image's red channel is used to determine heights. Terrain is 
		/// initially the same width and depth as the image, and 1 unit high. 
		/// </para>
		/// <para>
		/// Tips on generating nice terrain -
		/// </para>
		/// <para>
		/// * Smooth or blur the height map
		/// </para>
		/// <para>
		/// * Reduce the y scale of the terrain
		/// </para>
		/// <para>
		/// * Increase the x/z scale of the terrain
		/// </para>
		/// <para>
		/// * Reduce the camera range 
		/// </para>
		/// <para>
		/// When texturing an entity, a texture with a scale of 1,1,1
		/// (default) will be the same size as one of the terrain's grid squares. A
		/// texture that is scaled to the same size as the size of the bitmap used
		/// to load it or the no. of grid square used to create it, will be the same
		/// size as the terrain. 
		/// </para>
		/// <para>
		/// A heightmaps dimensions (width and height) must be the same and must be a 
		/// power of 2, e.g. 32, 64, 128, 256, 512, 1024. 
		/// </para>
		/// <para>
		/// The optional <c>parent</c> parameter attaches the new terrain
		/// to a parent entity. See the <c>EntityParent</c> command for 
		/// more details on the effects of entity parenting. 
		/// </para>
		/// </summary>
		/// <param name="file">filename of image file to be used as height map</param>
		/// <param name="parent">parent entity of terrain</param>
		public static int LoadTerrain(String file, int parent) { return bbLoadTerrain(file, parent); }
		public static int LoadTerrain(String file) { return bbLoadTerrain(file, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbTerrainDetail(int terrain, int detail_level, int vertex_morph);
		/// <summary>
		/// <para>
		/// Sets the detail level for a terrain. This is the number
		/// of triangles used to represent the terrain. A typical value is 2000.
		/// </para>
		/// <para>
		/// The optional <c>vertex_morph</c> parameter specifies whether to enable vertex
		/// morphing. It is recommended you set this to True, as it will reduce the
		/// visibility of LOD 'pop-in'. 
		/// </para>
		/// </summary>
		/// <param name="terrain">terrain handle</param>
		/// <param name="detail_level">detail level of terrain</param>
		/// <param name="vertex_morph">True to enable vertex morphing of terrain. Defaults to False.</param>
		public static int TerrainDetail(int terrain, int detail_level, int vertex_morph) { return bbTerrainDetail(terrain, detail_level, vertex_morph); }
		public static int TerrainDetail(int terrain, int detail_level) { return bbTerrainDetail(terrain, detail_level, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbTerrainShading(int terrain, int enable);
		/// <summary>
		/// <para>
		/// Enables or disables terrain shading.
		/// </para>
		/// <para>
		/// Shaded terrains are a little slower than non-shaded terrains, and in
		/// some instances can increase the visibility of LOD 'pop-in'. However, the
		/// option is there to have shaded terrains if you wish to do so. 
		/// </para>
		/// </summary>
		/// <param name="terrain">terrain handle</param>
		/// <param name="enable">True to enable terrain shading, False to to disable it. The default mode is False.</param>
		public static int TerrainShading(int terrain, int enable) { return bbTerrainShading(terrain, enable); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbModifyTerrain(int terrain, int grid_x, int grid_z, float height, int realtime);
		/// <summary>
		/// <para>
		/// Sets the height of a point on a terrain.
		/// </para>
		/// </summary>
		/// <param name="terrain">terrain handle</param>
		/// <param name="grid_x">grid x coordinate of terrain</param>
		/// <param name="grid_z">grid z coordinate of terrain</param>
		/// <param name="height">height of point on terrain. Should be in the range 0-1.</param>
		/// <param name="realtime">True to modify terrain immediately. False to modify terrain when RenderWorld in next called. Defaults to False.</param>
		public static int ModifyTerrain(int terrain, int grid_x, int grid_z, float height, int realtime) { return bbModifyTerrain(terrain, grid_x, grid_z, height, realtime); }
		public static int ModifyTerrain(int terrain, int grid_x, int grid_z, float height) { return bbModifyTerrain(terrain, grid_x, grid_z, height, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbTerrainSize(int terrain);
		/// <summary>
		/// <para>
		/// Returns the grid size used to create a terrain.
		/// </para>
		/// </summary>
		/// <param name="terrain">terrain handle</param>
		public static int TerrainSize(int terrain) { return bbTerrainSize(terrain); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbTerrainHeight(int terrain, int grid_x, int grid_z);
		/// <summary>
		/// <para>
		/// Returns the height of the terrain at terrain grid coordinates x,z.
		/// </para>
		/// <para>
		/// The value returned is in the range 0 to 1.
		/// </para>
		/// </summary>
		/// <param name="terrain">terrain handle</param>
		/// <param name="grid_x">grid x coordinate of terrain</param>
		/// <param name="grid_z">grid z coordinate of terrain</param>
		public static float TerrainHeight(int terrain, int grid_x, int grid_z) { return bbTerrainHeight(terrain, grid_x, grid_z); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbTerrainX(int terrain, float x, float y, float z);
		/// <summary>
		/// <para>
		/// Returns the interpolated x coordinate on a terrain.
		/// </para>
		/// </summary>
		/// <param name="terrain">terrain handle</param>
		/// <param name="x">world x coordinate</param>
		/// <param name="y">world y coordinate</param>
		/// <param name="z">world z coordinate</param>
		public static float TerrainX(int terrain, float x, float y, float z) { return bbTerrainX(terrain, x, y, z); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbTerrainY(int terrain, float x, float y, float z);
		/// <summary>
		/// <para>
		/// Returns the interpolated y coordinate on a terrain.
		/// </para>
		/// <para>
		/// <c>TerrainY</c> can be used to calculate the effective height of a
		/// terrain directly below / above the specified point.
		/// </para>
		/// </summary>
		/// <param name="terrain">terrain handle</param>
		/// <param name="x">world x coordinate</param>
		/// <param name="y">world y coordinate</param>
		/// <param name="z">world z coordinate</param>
		public static float TerrainY(int terrain, float x, float y, float z) { return bbTerrainY(terrain, x, y, z); }

		[DllImport("Blitz3D.dll")]
		public static extern float bbTerrainZ(int terrain, float x, float y, float z);
		/// <summary>
		/// <para>
		/// Returns the interpolated z coordinate on a terrain.
		/// </para>
		/// </summary>
		/// <param name="terrain">terrain handle</param>
		/// <param name="x">world x coordinate</param>
		/// <param name="y">world y coordinate</param>
		/// <param name="z">world z coordinate</param>
		public static float TerrainZ(int terrain, float x, float y, float z) { return bbTerrainZ(terrain, x, y, z); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbCreateSprite(int parent);
		/// <summary>
		/// <para>
		/// Creates a sprite entity and returns its handle.
		/// </para>
		/// <para>
		/// Sprites are 2D rectangles that can be oriented automatically
		/// towards the current rendering camera.
		/// </para>
		/// <para>
		/// Sprites are created at position (0,0,0) and extend from 
		/// (-1,-1,0) to (+1,+1,0) billboard style.
		/// </para>
		/// <para>
		/// Unlike other entities sprites are created with a default <c>EntityFX</c> 
		/// flag of 1 (FullBright).
		/// </para>
		/// <para>
		/// The orientation used to render a sprite unlike other entities
		/// is goverened by a combination of the sprite entities own position
		/// and orientation, the rendering camera's orientation and the 
		/// <c>SpriteViewMode.</c>
		/// </para>
		/// <para>
		/// The default viewmode of a sprite means it is always turned
		/// to face the camera. See the <c>SpriteViewMode</c> command for more
		/// information.
		/// </para>
		/// <para>
		/// The optional <c>parent</c> parameter attaches the new Sprite entity to a 
		/// specified parent entity. See the <c>EntityParent</c> command for more 
		/// information on entity hierachy.
		/// </para>
		/// <para>
		/// Unlike many Blitz3D primitives Sprites are not mesh based
		/// and must not have mesh based commands used on them. 
		/// </para>
		/// </summary>
		/// <param name="parent">optional parent entity of sprite</param>
		public static int CreateSprite(int parent) { return bbCreateSprite(parent); }
		public static int CreateSprite() { return bbCreateSprite(0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbLoadSprite(String path, int texture_flags, int parent);
		/// <summary>
		/// <para>
		/// Creates a sprite entity, and assigns a texture to it.
		/// </para>
		/// <list type="table">
		/// <listheader><term>Name</term><term>Value</term><term>Description</term></listheader>
		/// <item><description>TX_COLOR</description></item><item><description>1</description></item><item><description>Color (default)</description></item>
		/// <item><description>TX_ALPHA</description></item><item><description>2</description></item><item><description>Alpha</description></item>
		/// <item><description>TX_MASKED</description></item><item><description>4</description></item><item><description>Masked</description></item>
		/// <item><description>TX_MIP</description></item><item><description>8</description></item><item><description>Mipmapped</description></item>
		/// <item><description>TX_CLAMPU</description></item><item><description>16</description></item><item><description>Clamp U</description></item>
		/// <item><description>TX_CLAMPV</description></item><item><description>32</description></item><item><description>Clamp V</description></item>
		/// <item><description>TX_SPHERE</description></item><item><description>64</description></item><item><description>Spherical reflection map</description></item>
		/// <item><description>TX_CUBIC</description></item><item><description>128</description></item><item><description>Cubic environment map</description></item>
		/// <item><description>TX_VRAM</description></item><item><description>256</description></item><item><description>Store texture in vram</description></item>
		/// <item><description>TX_HIGHCOLOR</description></item><item><description>512</description></item><item><description>Force the use of high color textures</description></item>
		/// </list>
		/// <para>
		/// See the <c>CreateTexture</c> command for a detailed description
		/// of the texture flags.
		/// </para>
		/// <para>
		/// The optional <c>parent</c> parameter attaches the new Sprite entity to a 
		/// specified parent entity. See the <c>EntityParent</c> command for more 
		/// information on entity hierachy.
		/// </para>
		/// </summary>
		/// <param name="path">filename of image file to be used as sprite</param>
		/// <param name="tex_flag">optional texture flag</param>
		/// <param name="parent">optional parent of entity</param>
		public static int LoadSprite(String path, int texture_flags, int parent) { return bbLoadSprite(path, texture_flags, parent); }
		public static int LoadSprite(String path, int texture_flags) { return bbLoadSprite(path, texture_flags, 0); }
		public static int LoadSprite(String path) { return bbLoadSprite(path, 1, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbSpriteViewMode(int sprite, int view_mode);
		/// <summary>
		/// <para>
		/// Sets the view mode of a sprite.
		/// </para>
		/// <para>
		/// The view mode determines how at rendertime a sprite alters its 
		/// orientation in respect to the camera:
		/// </para>
		/// <list type="table">
		/// <listheader><term>Mode</term><term>Value</term><term>Description</term></listheader>
		/// <item><description>SPRITE_TURNXY</description></item><item><description>1</description></item><item><description>Turn about X and Y axis to face camera</description></item>
		/// <item><description>SPRITE_STILL</description></item><item><description>2</description></item><item><description>Do not modify orientation at render time.</description></item>
		/// <item><description>SPRITE_ALIGNZ</description></item><item><description>3</description></item><item><description>Turn about X and Y axis to face camera and align Z axis with camera</description></item>
		/// <item><description>SPRITE_TURNY</description></item><item><description>4</description></item><item><description>Turn about Y axis to face camera</description></item>
		/// </list>
		/// <para>
		/// This allows the sprite to in some instances give the impression
		/// that it is more than two dimensional. 
		/// </para>
		/// <para>
		/// In technical terms, the four sprite modes perform the following 
		/// changes: 
		/// </para>
		/// <para>
		/// Mode 1 - Sprite changes its pitch and yaw values to face camera, 
		/// but doesn't roll, good for most smoke and particle effects.
		/// </para>
		/// <para>
		/// Mode 2 - Sprite does not change either its pitch, yaw or roll 
		/// values, good for generic flat rectangular entities such as fences.
		/// </para>
		/// <para>
		/// Mode 3 - Sprite changes its yaw and pitch to face camera, and 
		/// changes its roll value to match cameras, useful for overlays.
		/// </para>
		/// <para>
		/// Mode 4 - Sprite changes its yaw to face camera, pitch and roll
		/// are unmodified. Useful for trees and other upstanding scenery.
		/// </para>
		/// <para>
		/// The <c>EntityFX</c> flag 16 can be used to make a Sprite double sided
		/// and hence visible from both sides. This applies to Mode 2 Sprites 
		/// in particular.
		/// </para>
		/// </summary>
		/// <param name="sprite">spritehandle</param>
		/// <param name="view_mode">view_mode of sprite</param>
		public static int SpriteViewMode(int sprite, int view_mode) { return bbSpriteViewMode(sprite, view_mode); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbRotateSprite(int sprite, float angle);
		/// <summary>
		/// <para>
		/// Rotates a sprite.
		/// </para>
		/// </summary>
		/// <param name="sprite">sprite handle</param>
		/// <param name="angle">absolute angle of sprite rotation</param>
		public static int RotateSprite(int sprite, float angle) { return bbRotateSprite(sprite, angle); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbScaleSprite(int sprite, float x_scale, float y_scale);
		/// <summary>
		/// <para>
		/// Scales a sprite.
		/// </para>
		/// </summary>
		/// <param name="sprite">sprite handle</param>
		/// <param name="x_scale">x scale of sprite</param>
		/// <param name="y_scale">y scale of sprite</param>
		public static int ScaleSprite(int sprite, float x_scale, float y_scale) { return bbScaleSprite(sprite, x_scale, y_scale); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbHandleSprite(int sprite, float x_handle, float y_handle);
		/// <summary>
		/// <para>
		/// Sets a sprite handle. 
		/// </para>
		/// <para>
		/// As a sprite extends from -1,-1 to +1,+1 and the handle
		/// defaults to 0,0 the standard handle of a sprite is
		/// the center of its image.
		/// </para>
		/// <para>
		/// A sprite's handle represents the relative position on the 
		/// sprite image used to position the sprite when 
		/// being rendered.
		/// </para>
		/// </summary>
		/// <param name="sprite">sprite handle.</param>
		/// <param name="x_handle">x handle of sprite</param>
		/// <param name="y_handle">y handle of sprite</param>
		public static int HandleSprite(int sprite, float x_handle, float y_handle) { return bbHandleSprite(sprite, x_handle, y_handle); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbLoadMD2(String md2_file, int parent);
		/// <summary>
		/// <para>
		/// Loads an MD2 entity and returns its handle.
		/// </para>
		/// <para>
		/// The MD2 model format uses a highly efficient vertex animation
		/// technology that is not compatible with the standard Blitz3D
		/// animation system but instead requires use of the specific MD2 
		/// animation commands.
		/// </para>
		/// <para>
		/// The optional <c>parent</c> parameter attaches the new MD2 entity to a 
		/// specified parent entity. See the <c>EntityParent</c> command for more 
		/// information on entity hierachy.
		/// </para>
		/// <para>
		/// An MD2 texture has to be loaded and applied separately, otherwise the md2 
		/// will appear untextured. 
		/// </para>
		/// </summary>
		/// <param name="md2_file">filename of md2</param>
		/// <param name="parent">parent entity of md2</param>
		public static int LoadMD2(String md2_file, int parent) { return bbLoadMD2(md2_file, parent); }
		public static int LoadMD2(String md2_file) { return bbLoadMD2(md2_file, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbAnimateMD2(int md2, int mode, float speed, int first_frame, int last_frame, float transition);
		/// <summary>
		/// <para>
		/// Animates an md2 entity.
		/// </para>
		/// <list type="table">
		/// <listheader><term>Mode</term><term>Value</term><term>Description</term></listheader>
		/// <item><description>ANIM_STOP</description></item><item><description>0</description></item><item><description>Stop animation</description></item>
		/// <item><description>ANIM_LOOP</description></item><item><description>1</description></item><item><description>Loop animation (default)</description></item>
		/// <item><description>ANIM_PINGPONG</description></item><item><description>2</description></item><item><description>PingPong animation</description></item>
		/// <item><description>ANIM_ONCE</description></item><item><description>3</description></item><item><description>OneShot animation</description></item>
		/// </list>
		/// <para>
		/// The MD2 will actually move from one frame to the next when UpdateWorld is called. 
		/// </para>
		/// </summary>
		/// <param name="md2">md2 handle</param>
		/// <param name="mode">mode of animation</param>
		/// <param name="speed">speed of animation. Defaults to 1.</param>
		/// <param name="first_frame">first frame of animation. Defaults to 1.</param>
		/// <param name="last_frame">last frame of animation. Defaults to last frame of all md2 animations.</param>
		/// <param name="transition">smoothness of transition between last frame shown of previous animation and first frame of next animation. Defaults to 0.</param>
		public static int AnimateMD2(int md2, int mode, float speed, int first_frame, int last_frame, float transition) { return bbAnimateMD2(md2, mode, speed, first_frame, last_frame, transition); }
		public static int AnimateMD2(int md2, int mode, float speed, int first_frame, int last_frame) { return bbAnimateMD2(md2, mode, speed, first_frame, last_frame, 0); }
		public static int AnimateMD2(int md2, int mode, float speed, int first_frame) { return bbAnimateMD2(md2, mode, speed, first_frame, 0, 0); }
		public static int AnimateMD2(int md2, int mode, float speed) { return bbAnimateMD2(md2, mode, speed, 1, 0, 0); }
		public static int AnimateMD2(int md2, int mode) { return bbAnimateMD2(md2, mode, 1.0f, 1, 0, 0); }
		public static int AnimateMD2(int md2) { return bbAnimateMD2(md2, 0, 1.0f, 1, 0, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbMD2AnimTime(int md2);
		/// <summary>
		/// <para>
		/// Returns the animation time of an md2 model.
		/// </para>
		/// <para>
		/// The animation time is the exact moment that the MD2 is at with
		/// regards its frames of animation.
		/// </para>
		/// <para>
		/// For example, if the MD2 entity is currently animating between
		/// the third and fourth frames, then MD2AnimTime will return a 
		/// number somewhere between 3 and 4. 
		/// </para>
		/// </summary>
		/// <param name="md2">md2 handle</param>
		public static int MD2AnimTime(int md2) { return bbMD2AnimTime(md2); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbMD2AnimLength(int md2);
		/// <summary>
		/// <para>
		/// Returns the animation length of an MD2 model in frames.
		/// </para>
		/// <para>
		/// The animation length is the total number of animation frames 
		/// loaded from the MD2 file.
		/// </para>
		/// </summary>
		/// <param name="md2">md2 handle</param>
		public static int MD2AnimLength(int md2) { return bbMD2AnimLength(md2); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbMD2Animating(int md2);
		/// <summary>
		/// <para>
		/// Returns <c>True</c> if the specified MD2 entity is currently animating,
		/// <c>False</c> if not.
		/// </para>
		/// </summary>
		/// <param name="md2">md2 handle</param>
		public static int MD2Animating(int md2) { return bbMD2Animating(md2); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbLoadBSP(String file, float gamma_adjust, int parent);
		/// <summary>
		/// <para>
		/// Loads a BSP model and returns its handle.
		/// </para>
		/// <para>
		/// A BSP model is a standard Blitz3D entity. Use the standard entity commands to
		/// scale, rotate and position the BSP, and the standard collision commands to 
		/// setup collisions with the BSP.
		/// </para>
		/// <para>
		/// BSP models are not lit by either <c>AmbientLight</c> or any directional
		/// lights. This allows you to setup lighting for in-game models without affecting 
		/// the BSP's internal lighting. BSP models ARE lit by point or spot lights.
		/// See the <c>BSPAmbientLight</c> and <c>BSPLighting</c> commands for more control
		/// over the lighting of BSP entities.
		/// </para>
		/// <para>
		/// BSP's cannot be painted, textured, colored, faded etc. in Blitz3D.
		/// </para>
		/// <para>
		/// Textures for the BSP model must be in the same directory as the BSP
		/// file itself. 
		/// </para>
		/// <para>
		/// Shaders are *not* supported!
		/// </para>
		/// <para>
		/// The optional <c>parent</c> parameter attaches the new Sprite entity to a 
		/// specified parent entity. See the <c>EntityParent</c> command for more 
		/// information on entity hierachy.
		/// </para>
		/// </summary>
		/// <param name="file">filename of BSP model</param>
		/// <param name="gamma_adjust">intensity of BSP lightmaps. Values should be in the range 0-1. Defaults to 0.</param>
		/// <param name="parent">parent entity of BSP</param>
		public static int LoadBSP(String file, float gamma_adjust, int parent) { return bbLoadBSP(file, gamma_adjust, parent); }
		public static int LoadBSP(String file, float gamma_adjust) { return bbLoadBSP(file, gamma_adjust, 0); }
		public static int LoadBSP(String file) { return bbLoadBSP(file, 0, 0); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbBSPAmbientLight(int bsp, float red, float green, float blue);
		/// <summary>
		/// <para>
		/// Sets the ambient lighting color for a BSP model.
		/// </para>
		/// <para>
		/// The red, green and blue values should be in the range 0-255. The default
		/// BSP ambient light color is 0,0,0.
		/// </para>
		/// <para>
		/// Note that BSP models do not use the <c>AmbientLight</c> setting. 
		/// </para>
		/// <para>
		/// This can also be used to increase the brightness of a BSP model,
		/// but the effect is not as 'nice' as using the <c>gamma_adjust</c> parameter of
		/// LoadBSP.
		/// </para>
		/// </summary>
		/// <param name="bsp">BSP handle</param>
		/// <param name="red">red BSP ambient light value</param>
		/// <param name="green">green BSP ambient light value</param>
		/// <param name="blue">blue BSP ambient light value</param>
		public static int BSPAmbientLight(int bsp, float red, float green, float blue) { return bbBSPAmbientLight(bsp, red, green, blue); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbBSPLighting(int bsp, int use_lightmaps);
		/// <summary>
		/// <para>
		/// Controls whether BSP models are illuminated using lightmaps, or by 
		/// vertex lighting.
		/// </para>
		/// <para>
		/// Vertex lighting will be faster on some graphics cards, but may not look 
		/// as good.
		/// </para>
		/// </summary>
		/// <param name="bsp">BSP handle</param>
		/// <param name="use_lightmaps">True to use lightmaps, False for vertex lighting. The default mode is True.</param>
		public static int BSPLighting(int bsp, int use_lightmaps) { return bbBSPLighting(bsp, use_lightmaps); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbCreateListener(int parent, float rolloff_factor, float doppler_scale, float distance_scale);
		/// <summary>
		/// <para>
		/// Creates a listener entity and returns its handle.
		/// </para>
		/// <para>
		/// Currently, only a single listener is supported which is typically parented
		/// to the program's main camera.
		/// </para>
		/// </summary>
		/// <param name="parent">parent entity of listener. A parent entity, typically a camera, must be specified to 'carry' the listener around.</param>
		/// <param name="rolloff_factor">the rate at which volume diminishes with distance. Defaults to 1.</param>
		/// <param name="doppler_scale">the severity of the doppler effect. Defaults to 1.</param>
		/// <param name="distance_scale">artificially scales distances. Defaults to 1.</param>
		public static int CreateListener(int parent, float rolloff_factor, float doppler_scale, float distance_scale) { return bbCreateListener(parent, rolloff_factor, doppler_scale, distance_scale); }
		public static int CreateListener(int parent, float rolloff_factor, float doppler_scale) { return bbCreateListener(parent, rolloff_factor, doppler_scale, 1.0f); }
		public static int CreateListener(int parent, float rolloff_factor) { return bbCreateListener(parent, rolloff_factor, 1.0f, 1.0f); }
		public static int CreateListener(int parent) { return bbCreateListener(parent, 1.0f, 1.0f, 1.0f); }

		[DllImport("Blitz3D.dll")]
		public static extern int bbEmitSound(int sound, int entity);
		/// <summary>
		/// <para>
		/// Emits a sound attached to the specified entity and returns a sound channel.
		/// </para>
		/// <para>
		/// The sound must have been loaded using <c>LoadSound</c> with the SOUND3D
		/// flag.
		/// </para>
		/// <para>
		/// The sound channel returned can subsequently be used with such sound channel 
		/// commands as <c>ChannelVolume</c> and <c>ChannelPitch.</c>
		/// </para>
		/// </summary>
		/// <param name="sound">sound handle</param>
		/// <param name="entity">entity handle</param>
		public static int EmitSound(int sound, int entity) { return bbEmitSound(sound, entity); }

		// constants from blitz3dsdk.h

		public const int BBFALSE = 0;
		public const int BBTRUE = 1;

		public const int GFX_DEFAULT = 0;
		public const int GFX_FULLSCREEN = 1;
		public const int GFX_WINDOWED = 2;
		public const int GFX_WINDOWEDSCALED = 3;

		public const int PROJ_NONE = 0;
		public const int PROJ_PERSPECTIVE = 1;
		public const int PROJ_ORTHO = 2;

		public const int LIGHT_DIRECTIONAL = 1;
		public const int LIGHT_POINT = 2;
		public const int LIGHT_SPOT = 3;

		public const int TX_COLOR = 1;
		public const int TX_ALPHA = 2;
		public const int TX_MASKED = 4;
		public const int TX_MIP = 8;
		public const int TX_CLAMPU = 16;
		public const int TX_CLAMPV = 32;
		public const int TX_SPHERE = 64;
		public const int TX_CUBIC = 128;
		public const int TX_VRAM = 256;
		public const int TX_HIGHCOLOR = 512;

		public const int TX_BLEND_NONE = 0;
		public const int TX_BLEND_ALPHA = 1;
		public const int TX_BLEND_MULT = 2;
		public const int TX_BLEND_ADD = 3;
		public const int TX_BLEND_DOT3 = 4;
		public const int TX_BLEND_MULT2 = 5;

		public const int CUBEFACE_LEFT = 0;
		public const int CUBEFACE_FRONT = 1;
		public const int CUBEFACE_RIGHT = 2;
		public const int CUBEFACE_BACK = 3;
		public const int CUBEFACE_TOP = 4;
		public const int CUBEFACE_BOTTOM = 5;

		public const int CUBEMODE_SPECULAR = 1;
		public const int CUBEMODE_DIFFUSE = 2;
		public const int CUBEMODE_REFRACTION = 3;

		public const int BRUSHBLEND_NONE = 0;
		public const int BRUSHBLEND_ALPHA = 1;
		public const int BRUSHBLEND_MULTIPLY = 2;
		public const int BRUSHBLEND_ADD = 3;

		public const int BRUSHFX_NONE = 0;
		public const int BRUSHFX_FULLBRIGHT = 1;
		public const int BRUSHFX_VERTEXCOLOR = 2;
		public const int BRUSHFX_FLAT = 4;
		public const int BRUSHFX_NOFOG = 8;
		public const int BRUSHFX_DOUBLESIDED = 16;
		public const int BRUSHFX_VERTEXALPHA = 32;

		public const int COLLIDE_SPHERESPHERE = 1;
		public const int COLLIDE_SPHEREPOLY = 2;
		public const int COLLIDE_SPHEREBOX = 3;

		public const int COLLIDE_STOP = 1;
		public const int COLLIDE_SLIDE1 = 2;
		public const int COLLIDE_SLIDE2 = 3;

		public const int PICK_NONE = 0;
		public const int PICK_SPHERE = 1;
		public const int PICK_POLY = 2;
		public const int PICK_BOX = 3;

		public const int ANIM_STOP = 0;
		public const int ANIM_LOOP = 1;
		public const int ANIM_PINGPONG = 2;
		public const int ANIM_ONCE = 3;

		public const int SPRITE_TURNXY = 1;
		public const int SPRITE_STILL = 2;
		public const int SPRITE_ALIGNZ = 3;
		public const int SPRITE_TURNY = 4;

		public const int PLAYCD_SINGLE = 1;
		public const int PLAYCD_LOOP = 2;
		public const int PLAYCD_ALL = 3;

		public const int MOUSE_BUTTON = 1;
		public const int MOUSE_RIGHTBUTTON = 2;
		public const int MOUSE_MIDDLEBUTTON = 3;

		public const int JOYTYPE_NONE = 0;
		public const int JOYTYPE_DIGITAL = 1;
		public const int JOYTYPE_ANALOG = 2;

		public const int KEY_ESCAPE = 1;
		public const int KEY_TAB = 15;
		public const int KEY_SPACE = 57;
		public const int KEY_RETURN = 28;
		public const int KEY_LEFTSHIFT = 42;
		public const int KEY_LEFTCONTROL = 29;
		public const int KEY_LEFTALT = 56;
		public const int KEY_RIGHTSHIFT = 54;
		public const int KEY_RIGHTCONTROL = 157;
		public const int KEY_RIGHTALT = 184;

		public const int KEY_UP = 200;
		public const int KEY_LEFT = 203;
		public const int KEY_RIGHT = 205;
		public const int KEY_DOWN = 208;

		public const int KEY_F1 = 59;
		public const int KEY_F2 = 60;
		public const int KEY_F3 = 61;
		public const int KEY_F4 = 62;
		public const int KEY_F5 = 63;
		public const int KEY_F6 = 64;
		public const int KEY_F7 = 65;
		public const int KEY_F8 = 66;
		public const int KEY_F9 = 67;
		public const int KEY_F10 = 68;

		public const int KEY_Q = 16;
		public const int KEY_W = 17;
		public const int KEY_E = 18;
		public const int KEY_R = 19;
		public const int KEY_T = 20;
		public const int KEY_Y = 21;
		public const int KEY_U = 22;
		public const int KEY_I = 23;
		public const int KEY_O = 24;
		public const int KEY_P = 25;

		public const int KEY_A = 30;
		public const int KEY_S = 31;
		public const int KEY_D = 32;
		public const int KEY_F = 33;
		public const int KEY_G = 34;
		public const int KEY_H = 35;
		public const int KEY_J = 36;
		public const int KEY_K = 37;
		public const int KEY_L = 38;

		public const int KEY_Z = 44;
		public const int KEY_X = 45;
		public const int KEY_C = 46;
		public const int KEY_V = 47;
		public const int KEY_B = 48;
		public const int KEY_N = 49;
		public const int KEY_M = 50;

		public const int KEY_1 = 2;
		public const int KEY_2 = 3;
		public const int KEY_3 = 4;
		public const int KEY_4 = 5;
		public const int KEY_5 = 6;
		public const int KEY_6 = 7;
		public const int KEY_7 = 8;
		public const int KEY_8 = 9;
		public const int KEY_9 = 10;
		public const int KEY_0 = 11;

		public const int KEY_MINUS = 12;
		public const int KEY_EQUALS = 13;
		public const int KEY_BACKSPACE = 14;
		public const int KEY_LEFTBRACKET = 26;
		public const int KEY_RIGHTBRACKET = 27;
		public const int KEY_SEMICOLON = 39;
		public const int KEY_APOSTROPHE = 40;
		public const int KEY_GRAVE = 41;
		public const int KEY_BACKSLASH = 43;
		public const int KEY_COMMA = 51;
		public const int KEY_PERIOD = 52;
		public const int KEY_SLASH = 53;
		public const int KEY_CAPSLOCK = 58;

		public const int KEY_PRINT = 183;
		public const int KEY_SCROLLLOCK = 70;
		public const int KEY_PAUSEBREAK = 197;
		public const int KEY_INSERT = 210;
		public const int KEY_DELETE = 211;
		public const int KEY_HOME = 199;
		public const int KEY_END = 207;
		public const int KEY_PAGEUP = 201;
		public const int KEY_PAGEDOWN = 209;

		public const int KEY_NUMLOCK = 69;
		public const int KEY_NUMPADDIVIDE = 181;
		public const int KEY_NUMPADMULT = 55;
		public const int KEY_NUMPADMINUS = 74;
		public const int KEY_NUMPADPLUS = 78;
		public const int KEY_NUMPADENTER = 156;
		public const int KEY_NUMPADDOT = 83;
		public const int KEY_NUMPAD0 = 82;
		public const int KEY_NUMPAD1 = 79;
		public const int KEY_NUMPAD2 = 80;
		public const int KEY_NUMPAD3 = 81;
		public const int KEY_NUMPAD4 = 75;
		public const int KEY_NUMPAD5 = 76;
		public const int KEY_NUMPAD6 = 77;
		public const int KEY_NUMPAD7 = 71;
		public const int KEY_NUMPAD8 = 72;
		public const int KEY_NUMPAD9 = 73;

		public const int KEY_F11 = 87;
		public const int KEY_F12 = 88;
		public const int KEY_LEFTWINDOWS = 219;
		public const int KEY_RIGHTWINDOWS = 220;
		public const int KEY_NUMPADEQUALS = 141;
		public const int KEY_NUMPADCOMMA = 179;
		public const int KEY_OEM_102 = 86;
		public const int KEY_AT = 145;
		public const int KEY_COLON = 146;
		public const int KEY_UNDERLINE = 147;
		public const int KEY_BREAK = 149;

		public const int KEY_PREVIOUSTRACK = 144;
		public const int KEY_NEXTTRACK = 153;
		public const int KEY_MUTE = 160;
		public const int KEY_PLAYPAUSE = 162;
		public const int KEY_STOP = 164;
		public const int KEY_VOLUMELESS = 174;
		public const int KEY_VOLUMEMORE = 176;

		public const int KEY_APPS = 221;
		public const int KEY_POWER = 222;
		public const int KEY_SLEEP = 223;
		public const int KEY_WAKE = 227;

		public const int KEY_WEBHOME = 178;
		public const int KEY_WEBSEARCH = 229;
		public const int KEY_WEBFAVORITES = 230;
		public const int KEY_WEBREFRESH = 231;
		public const int KEY_WEBSTOP = 232;
		public const int KEY_WEBFORWARD = 233;
		public const int KEY_WEBBACK = 234;

		public const int KEY_MYCOMPUTER = 235;
		public const int KEY_CALCULATOR = 161;
		public const int KEY_MAIL = 236;
		public const int KEY_MEDIASELECT = 237;
	}
}