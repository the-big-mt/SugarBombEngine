/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2016-2017 Dustin Land
Copyright (C) 2017 Robert Beckebans
Copyright (C) 2019 BlackPhrase

This file is part of the Doom 3 BFG Edition GPL Source Code ("Doom 3 BFG Edition Source Code").

Doom 3 BFG Edition Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 BFG Edition Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 BFG Edition Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 BFG Edition Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

#pragma once

#include "RenderCommon.h"
#include "ScreenRect.h"

#include "idlib/Str.h"
#include "idlib/math/Vector.h"
#include "idlib/math/Matrix.h"
#include "idlib/geometry/Winding.h"
#include "idlib/geometry/RenderMatrix.h"
#include "idlib/sys/sys_types.h"
#include "idlib/sys/sys_defines.h"

// RB begin
#define USE_CORE_PROFILE

bool GL_CheckErrors_(const char *filename, int line);
#if 1 // !defined(RETAIL)
#define GL_CheckErrors() GL_CheckErrors_(__FILE__, __LINE__)
#else
#define GL_CheckErrors() false
#endif
// RB end

struct textureStage_t;
struct shaderStage_t;
struct emptyCommand_t;
struct viewLight_t;
struct drawInteraction_t;
class idPlane;
enum stageVertexColor_t : int;

// TODO: cullType_t;
// TODO: viewEntity_t
// TODO: Framebuffer
// TODO: vertexLayoutType_t
// TODO: graphicsVendor_t

struct tmu_t
{
	unsigned int current2DMap;
	unsigned int current2DArray;
	unsigned int currentCubeMap;
};

const int MAX_MULTITEXTURE_UNITS = 8;

enum stencilFace_t
{
	STENCIL_FACE_FRONT,
	STENCIL_FACE_BACK,
	STENCIL_FACE_NUM
};

struct backEndCounters_t
{
	int c_surfaces;
	int c_shaders;

	int c_drawElements;
	int c_drawIndexes;

	int c_shadowElements;
	int c_shadowIndexes;

	int c_copyFrameBuffer;

	float c_overDraw;

	int totalMicroSec; // total microseconds for backend run
	int shadowMicroSec;
};

struct gfxImpParms_t
{
	int x; // ignored in fullscreen
	int y; // ignored in fullscreen
	int width;
	int height;
	int fullScreen; // 0 = windowed, otherwise 1 based monitor number to go full screen on
	// -1 = borderless window for spanning multiple displays
	int displayHz;
	int multiSamples;
};

#define MAX_DEBUG_LINES 16384
#define MAX_DEBUG_TEXT 512
#define MAX_DEBUG_POLYGONS 8192

struct debugLine_t
{
	idVec4 rgb;
	idVec3 start;
	idVec3 end;
	bool depthTest;
	int lifeTime;
};

struct debugText_t
{
	idStr text;
	idVec3 origin;
	float scale;
	idVec4 color;
	idMat3 viewAxis;
	int align;
	int lifeTime;
	bool depthTest;
};

struct debugPolygon_t
{
	idVec4 rgb;
	idWinding winding;
	bool depthTest;
	int lifeTime;
};

void RB_SetMVP(const idRenderMatrix &mvp);
void RB_SetVertexColorParms(stageVertexColor_t svc);
void RB_GetShaderTextureMatrix(const float *shaderRegisters, const textureStage_t *texture, float matrix[16]);
void RB_LoadShaderTextureMatrix(const float *shaderRegisters, const textureStage_t *texture);
void RB_BakeTextureMatrixIntoTexgen(idPlane lightProject[3], const float *textureMatrix);

//bool ChangeDisplaySettingsIfNeeded( gfxImpParms_t parms );
//bool CreateGameWindow( gfxImpParms_t parms );

#if defined(ID_VULKAN)

struct gpuInfo_t
{
	VkPhysicalDevice device;
	VkPhysicalDeviceProperties props;
	VkPhysicalDeviceMemoryProperties memProps;
	VkSurfaceCapabilitiesKHR surfaceCaps;
	idList<VkSurfaceFormatKHR> surfaceFormats;
	idList<VkPresentModeKHR> presentModes;
	idList<VkQueueFamilyProperties> queueFamilyProps;
	idList<VkExtensionProperties> extensionProps;
};

struct vulkanContext_t
{
	uint64 counter;
	uint32 currentFrameData;

	vertCacheHandle_t jointCacheHandle;
	uint64 stencilOperations[STENCIL_FACE_NUM];

	VkInstance instance;
	VkPhysicalDevice physicalDevice;
	VkPhysicalDeviceFeatures physicalDeviceFeatures;
	VkDevice device;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	int graphicsFamilyIdx;
	int presentFamilyIdx;
	VkDebugReportCallbackEXT callback;

	idList<const char *> instanceExtensions;
	idList<const char *> deviceExtensions;
	idList<const char *> validationLayers;

	gpuInfo_t *gpu;
	idList<gpuInfo_t> gpus;

	VkCommandPool commandPool;
	idArray<VkCommandBuffer, NUM_FRAME_DATA> commandBuffer;
	idArray<VkFence, NUM_FRAME_DATA> commandBufferFences;
	idArray<bool, NUM_FRAME_DATA> commandBufferRecorded;

	VkSurfaceKHR surface;
	VkPresentModeKHR presentMode;
	VkFormat depthFormat;
	VkRenderPass renderPass;
	VkPipelineCache pipelineCache;
	VkSampleCountFlagBits sampleCount;
	bool supersampling;

	int fullscreen;
	VkSwapchainKHR swapchain;
	VkFormat swapchainFormat;
	VkExtent2D swapchainExtent;
	uint32 currentSwapIndex;
	VkImage msaaImage;
	VkImageView msaaImageView;
#if defined(ID_USE_AMD_ALLOCATOR)
	VmaAllocation msaaVmaAllocation;
	VmaAllocationInfo msaaAllocation;
#else
	vulkanAllocation_t msaaAllocation;
#endif
	idArray<idImage *, NUM_FRAME_DATA> swapchainImages;
	idArray<VkFramebuffer, NUM_FRAME_DATA> frameBuffers;
	idArray<VkSemaphore, NUM_FRAME_DATA> acquireSemaphores;
	idArray<VkSemaphore, NUM_FRAME_DATA> renderCompleteSemaphores;

	int currentImageParm;
	idArray<idImage *, MAX_IMAGE_PARMS> imageParms;
};

extern vulkanContext_t vkcontext;

#else //if defined( ID_OPENGL )

struct glContext_t
{
	//	bool		bAnisotropicFilterAvailable;
	//	bool		bTextureLODBiasAvailable;

	//	float		maxTextureAnisotropy;

	tmu_t tmu[MAX_MULTITEXTURE_UNITS];
	uint64 stencilOperations[STENCIL_FACE_NUM];
};

extern glContext_t glcontext;

#endif

/*
===========================================================================

idRenderBackend

all state modified by the back end is separated from the front end state

===========================================================================
*/
class idRenderBackend
{
	friend class Framebuffer;

public:
	idRenderBackend();
	~idRenderBackend();

	void Init();
	void Shutdown();

	void ExecuteBackEndCommands(const emptyCommand_t *cmds);
	void StereoRenderExecuteBackEndCommands(const emptyCommand_t *const allCmds);
	void BlockingSwapBuffers();

	void Print();
	void CheckCVars();

private:
	void DrawFlickerBox();

	void DrawElementsWithCounters(const drawSurf_t *surf);
	void DrawStencilShadowPass(const drawSurf_t *drawSurf, const bool renderZPass);

	void SetColorMappings();
	void ResizeImages();

	void DrawViewInternal(const viewDef_t *viewDef, const int stereoEye);
	void DrawView(const void *data, const int stereoEye);
	void CopyRender(const void *data);

	void BindVariableStageImage(const textureStage_t *texture, const float *shaderRegisters);
	void PrepareStageTexturing(const shaderStage_t *pStage, const drawSurf_t *surf);
	void FinishStageTexturing(const shaderStage_t *pStage, const drawSurf_t *surf);

	void ResetViewportAndScissorToDefaultCamera(const viewDef_t *_viewDef);

	void FillDepthBufferGeneric(const drawSurf_t *const *drawSurfs, int numDrawSurfs);
	void FillDepthBufferFast(drawSurf_t **drawSurfs, int numDrawSurfs);

	void T_BlendLight(const drawSurf_t *drawSurfs, const viewLight_t *vLight);
	void BlendLight(const drawSurf_t *drawSurfs, const drawSurf_t *drawSurfs2, const viewLight_t *vLight);
	void T_BasicFog(const drawSurf_t *drawSurfs, const idPlane fogPlanes[4], const idRenderMatrix *inverseBaseLightProject);
	void FogPass(const drawSurf_t *drawSurfs, const drawSurf_t *drawSurfs2, const viewLight_t *vLight);
	void FogAllLights();

	void SetupInteractionStage(const shaderStage_t *surfaceStage, const float *surfaceRegs, const float lightColor[4],
	                           idVec4 matrix[2], float color[4]);

	void DrawInteractions(const viewDef_t *_viewDef);
	void DrawSingleInteraction(drawInteraction_t *din);
	int DrawShaderPasses(const drawSurf_t *const *const drawSurfs, const int numDrawSurfs,
	                     const float guiStereoScreenOffset, const int stereoEye);

	void RenderInteractions(const drawSurf_t *surfList, const viewLight_t *vLight, int depthFunc, bool performStencilTest, bool useLightDepthBounds);

	// RB
	void AmbientPass(const drawSurf_t *const *drawSurfs, int numDrawSurfs, bool fillGbuffer);
	void ShadowMapPass(const drawSurf_t *drawSurfs, const viewLight_t *vLight, int side);

	void StencilShadowPass(const drawSurf_t *drawSurfs, const viewLight_t *vLight);
	void StencilSelectLight(const viewLight_t *vLight);

	// RB: HDR stuff

	// TODO optimize and replace with compute shader
	void CalculateAutomaticExposure();
	void Tonemap(const viewDef_t *viewDef);
	void Bloom(const viewDef_t *viewDef);

	void DrawScreenSpaceAmbientOcclusion(const viewDef_t *_viewDef);
	void DrawScreenSpaceGlobalIllumination(const viewDef_t *_viewDef);

	// Experimental feature
	void MotionBlur();
	void PostProcess(const void *data);

private:
	void GL_StartFrame();
	void GL_EndFrame();

	uint64 GL_GetCurrentState() const;
	uint64 GL_GetCurrentStateMinusStencil() const;
	void GL_SetDefaultState();

	void GL_State(uint64 stateBits, bool forceGlState = false);
	void GL_SeparateStencil(stencilFace_t face, uint64 stencilBits);
	void GL_Cull(cullType_t cullType); // TODO remove

	void GL_SelectTexture(int unit);
	//	void				GL_BindTexture( idImage* image );

	//	void				GL_CopyFrameBuffer( idImage* image, int x, int y, int imageWidth, int imageHeight );
	//	void				GL_CopyDepthBuffer( idImage* image, int x, int y, int imageWidth, int imageHeight );

	// RB: HDR parm
	void GL_Clear(bool color, bool depth, bool stencil, byte stencilValue, float r, float g, float b, float a, bool clearHDR = true);

	void GL_DepthBoundsTest(const float zmin, const float zmax);
	void GL_PolygonOffset(float scale, float bias);

	void GL_Scissor(int x /* left*/, int y /* bottom */, int w, int h);
	void GL_Viewport(int x /* left */, int y /* bottom */, int w, int h);

	ID_INLINE void GL_Scissor(const idScreenRect &rect)
	{
		GL_Scissor(rect.x1, rect.y1, rect.x2 - rect.x1 + 1, rect.y2 - rect.y1 + 1);
	}
	ID_INLINE void GL_Viewport(const idScreenRect &rect)
	{
		GL_Viewport(rect.x1, rect.y1, rect.x2 - rect.x1 + 1, rect.y2 - rect.y1 + 1);
	}

	ID_INLINE void GL_ViewportAndScissor(int x, int y, int w, int h)
	{
		GL_Viewport(x, y, w, h);
		GL_Scissor(x, y, w, h);
	}

	ID_INLINE void GL_ViewportAndScissor(const idScreenRect &rect)
	{
		GL_Viewport(rect);
		GL_Scissor(rect);
	}

	void GL_Color(float r, float g, float b, float a);
	ID_INLINE void GL_Color(float r, float g, float b)
	{
		GL_Color(r, g, b, 1.0f);
	}

	ID_INLINE void GL_Color(const idVec3 &color)
	{
		GL_Color(color[0], color[1], color[2], 1.0f);
	}

	ID_INLINE void GL_Color(const idVec4 &color)
	{
		GL_Color(color[0], color[1], color[2], color[3]);
	}

	//	void				GL_Color( float* color );

	void SetBuffer(const void *data);

private:
	void DBG_SimpleSurfaceSetup(const drawSurf_t *drawSurf);
	void DBG_SimpleWorldSetup();
	void DBG_PolygonClear();
	void DBG_ShowDestinationAlpha();
	void DBG_ScanStencilBuffer();
	void DBG_CountStencilBuffer();
	void DBG_ColorByStencilBuffer();
	void DBG_ShowOverdraw();
	void DBG_ShowIntensity();
	void DBG_ShowDepthBuffer();
	void DBG_ShowLightCount();
	//	void				DBG_EnterWeaponDepthHack();
	//	void				DBG_EnterModelDepthHack( float depth );
	//	void				DBG_LeaveDepthHack();
	void DBG_RenderDrawSurfListWithFunction(drawSurf_t **drawSurfs, int numDrawSurfs);
	void DBG_ShowSilhouette();
	void DBG_ShowTris(drawSurf_t **drawSurfs, int numDrawSurfs);
	void DBG_ShowSurfaceInfo(drawSurf_t **drawSurfs, int numDrawSurfs);
	void DBG_ShowViewEntitys(viewEntity_t *vModels);
	void DBG_ShowTexturePolarity(drawSurf_t **drawSurfs, int numDrawSurfs);
	void DBG_ShowUnsmoothedTangents(drawSurf_t **drawSurfs, int numDrawSurfs);
	void DBG_ShowTangentSpace(drawSurf_t **drawSurfs, int numDrawSurfs);
	void DBG_ShowVertexColor(drawSurf_t **drawSurfs, int numDrawSurfs);
	void DBG_ShowNormals(drawSurf_t **drawSurfs, int numDrawSurfs);
	void DBG_ShowTextureVectors(drawSurf_t **drawSurfs, int numDrawSurfs);
	void DBG_ShowDominantTris(drawSurf_t **drawSurfs, int numDrawSurfs);
	void DBG_ShowEdges(drawSurf_t **drawSurfs, int numDrawSurfs);
	void DBG_ShowLights();
	void DBG_ShowShadowMapLODs(); // RB
	void DBG_ShowPortals();

	void DBG_ShowDebugText();
	void DBG_ShowDebugLines();
	void DBG_ShowDebugPolygons();

	void DBG_ShowCenterOfProjection();
	void DBG_ShowLines();

	void DBG_TestGamma();
	void DBG_TestGammaBias();
	void DBG_TestImage();
	void DBG_ShowShadowMaps(); // RB
	void DBG_ShowTrace(drawSurf_t **drawSurfs, int numDrawSurfs);
	void DBG_RenderDebugTools(drawSurf_t **drawSurfs, int numDrawSurfs);

public:
	backEndCounters_t pc;

	// surfaces used for code-based drawing
	drawSurf_t unitSquareSurface;
	drawSurf_t zeroOneCubeSurface;
	drawSurf_t testImageSurface;

private:
	uint64 glStateBits;

	const viewDef_t *viewDef;

	const viewEntity_t *currentSpace; // for detecting when a matrix must change
	idScreenRect currentScissor;      // for scissor clipping, local inside renderView viewport

	bool currentRenderCopied; // true if any material has already referenced _currentRender

	idRenderMatrix prevMVP[2]; // world MVP from previous frame for motion blur

	// RB begin
	idRenderMatrix shadowV[6]; // shadow depth view matrix
	idRenderMatrix shadowP[6]; // shadow depth projection matrix

	float hdrAverageLuminance;
	float hdrMaxLuminance;
	float hdrTime;
	float hdrKey;
	// RB end

private:
#if !defined(USE_VULKAN)
	int currenttmu;

	unsigned int currentVertexBuffer;
	unsigned int currentIndexBuffer;
	Framebuffer *currentFramebuffer; // RB: for offscreen rendering

	int faceCulling;
	vertexLayoutType_t vertexLayout;

	float polyOfsScale;
	float polyOfsBias;

public:
	int GetCurrentTextureUnit() const
	{
		return currenttmu;
	}

#if 0
	unsigned short		gammaTable[ 256 ];		// brightness / gamma modify this
	
	idStr				rendererString;
	idStr				vendorString;
	idStr				versionString;
	idStr				extensionsString;
	idStr				wglExtensionsString;
	idStr				shadingLanguageString;
	
	float				glVersion;			// atof( version_string )
	graphicsVendor_t	vendor;
	
	int					maxTextureSize;		// queried from GL
	int					maxTextureCoords;
	int					maxTextureImageUnits;
	int					uniformBufferOffsetAlignment;
	
	int					colorBits;
	int					depthBits;
	int					stencilBits;
	
	bool				depthBoundsTestAvailable;
	bool				timerQueryAvailable;
	bool				swapControlTearAvailable;
	
	int					displayFrequency;
#endif

#endif // !defined( USE_VULKAN )
};