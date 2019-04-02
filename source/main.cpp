#include <3ds.h>
#include "render.h"

#define CLEAR_COLOR 0x68B0D8FF

#define DISPLAY_TRANSFER_FLAGS \
	(GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(0) | GX_TRANSFER_RAW_COPY(0) | \
	GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8) | \
	GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO))

float angleX = 0.0f;
float angleZ = 0.0f;

int main()
{
	// Initialize graphics
	gfxInitDefault();
    consoleInit(GFX_BOTTOM, nullptr);

    printf("Console Attached\n");
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);

	// Initialize the render target
	C3D_RenderTarget* target = C3D_RenderTargetCreate(240, 400, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
	C3D_RenderTargetSetOutput(target, GFX_TOP, GFX_LEFT, DISPLAY_TRANSFER_FLAGS);

	Render renderer;
    printf("Loading FACE\n");
	renderer.model.Load("Face", "/cube.obj");

	printf("Binding to BufInfo\n");
	renderer.model.BindToBufInfo(renderer.GetVBOVector());

	C3D_Mtx *modelView = renderer.model.GetModelViewMtx("Face");
	Mtx_Identity(modelView);
	Mtx_Translate(modelView, 0.0, -10.0, -120.0f, true);

	printf("Loading FACE2\n");
	renderer.model.Load("Nose", "/nose.obj");
	renderer.model.BindToBufInfo(renderer.GetVBOVector());
	//modelView = renderer.model.GetModelViewMtx("Nose");
	//Mtx_Identity(modelView);
	//Mtx_Translate(modelView, 0.0, -60.0, -120.0f, true);

	printf("Binding to BufInfo\n");
	

	//renderer.model.Load("N")

	// Main loop
	while (aptMainLoop())
	{
		hidScanInput();

		// Respond to user input
		u32 kDown = hidKeysDown();
		if(kDown & KEY_A)
			printf("Angle X = %f\n", angleX+=0.5f);

		if(kDown & KEY_B)
			printf("Scale factor Z = %f\n", angleZ+=0.5f);

		if (kDown & KEY_START)
			break; // break in order to return to hbmenu
		// Render the scene
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C3D_RenderTargetClear(target, C3D_CLEAR_ALL, CLEAR_COLOR, 0);
			C3D_FrameDrawOn(target);
			renderer.Draw();
		C3D_FrameEnd(0);
	}

	// Deinitialize graphics
	C3D_Fini();
	gfxExit();
	return 0;
}
