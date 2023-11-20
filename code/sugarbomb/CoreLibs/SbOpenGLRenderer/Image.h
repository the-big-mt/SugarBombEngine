/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2013-2017 Robert Beckebans
Copyright (C) 2016-2017 Dustin Land
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

//#include <GL/glew.h> // TODO

#include <cstring> // memcmp

#include "idlib/sys/sys_types.h"
#include "idlib/sys/sys_defines.h"
#include "idlib/containers/List.h"
#include "idlib/containers/HashIndex.h"
#include "idlib/Str.h"

#include "Material.h" // textureFilter_t, textureRepeat_t

struct MemInfo_t;
struct idCommon;
struct idCmdSystem;
struct idFileSystem;
struct idDeclManager;
class idPreloadManifest;
class idLexer;

enum textureType_t
{
	TT_DISABLED,
	TT_2D,
	TT_CUBIC,
	// RB begin
	TT_2D_ARRAY,
	TT_2D_MULTISAMPLE
	// RB end
};

/*
================================================
The internal *Texture Format Types*, ::textureFormat_t, are:
================================================
*/
enum textureFormat_t : int
{
	FMT_NONE,
	
	//------------------------
	// Standard color image formats
	//------------------------
	
	FMT_RGBA8,			// 32 bpp
	FMT_XRGB8,			// 32 bpp
	
	//------------------------
	// Alpha channel only
	//------------------------
	
	// Alpha ends up being the same as L8A8 in our current implementation, because straight
	// alpha gives 0 for color, but we want 1.
	FMT_ALPHA,
	
	//------------------------
	// Luminance replicates the value across RGB with a constant A of 255
	// Intensity replicates the value across RGBA
	//------------------------
	
	FMT_L8A8,			// 16 bpp
	FMT_LUM8,			//  8 bpp
	FMT_INT8,			//  8 bpp
	
	//------------------------
	// Compressed texture formats
	//------------------------
	
	FMT_DXT1,			// 4 bpp
	FMT_DXT5,			// 8 bpp
	
	//------------------------
	// Depth buffer formats
	//------------------------
	
	FMT_DEPTH,			// 24 bpp
	
	//------------------------
	//
	//------------------------
	
	FMT_X16,			// 16 bpp
	FMT_Y16_X16,		// 32 bpp
	FMT_RGB565,			// 16 bpp
	
	// RB: don't change above for legacy .bimage compatibility
	FMT_ETC1_RGB8_OES,	// 4 bpp
	FMT_SHADOW_ARRAY,	// 32 bpp * 6
	FMT_RGBA16F,		// 64 bpp
	FMT_RGBA32F,		// 128 bpp
	FMT_R32F,			// 32 bpp
	// RB end
};

int BitsForFormat( textureFormat_t format );

enum textureSamples_t
{
	SAMPLE_1	= BIT( 0 ),
	SAMPLE_2	= BIT( 1 ),
	SAMPLE_4	= BIT( 2 ),
	SAMPLE_8	= BIT( 3 ),
	SAMPLE_16	= BIT( 4 )
};

/*
================================================
DXT5 color formats
================================================
*/
enum textureColor_t : int
{
	CFM_DEFAULT,			// RGBA
	CFM_NORMAL_DXT5,		// XY format and use the fast DXT5 compressor
	CFM_YCOCG_DXT5,			// convert RGBA to CoCg_Y format
	CFM_GREEN_ALPHA,		// Copy the alpha channel to green
	
	// RB: don't change above for legacy .bimage compatibility
	CFM_YCOCG_RGBA8,
	// RB end
};

/*
================================================
idImageOpts hold parameters for texture operations.
================================================
*/
class idImageOpts
{
public:
	idImageOpts();
	
	bool	operator==( const idImageOpts& opts );
	
	//---------------------------------------------------
	// these determine the physical memory size and layout
	//---------------------------------------------------
	
	textureType_t		textureType;
	textureFormat_t		format;
	textureColor_t		colorFormat;
	textureSamples_t	samples;
	int					width;
	int					height;			// not needed for cube maps
	int					numLevels;		// if 0, will be 1 for NEAREST / LINEAR filters, otherwise based on size
	bool				gammaMips;		// if true, mips will be generated with gamma correction
	bool				readback;		// 360 specific - cpu reads back from this texture, so allocate with cached memory
};

/*
========================
idImageOpts::idImageOpts
========================
*/
ID_INLINE idImageOpts::idImageOpts()
{
	format			= FMT_NONE;
	colorFormat		= CFM_DEFAULT;
	samples			= SAMPLE_1;
	width			= 0;
	height			= 0;
	numLevels		= 0;
	textureType		= TT_2D;
	gammaMips		= false;
	readback		= false;
	
};

/*
========================
idImageOpts::operator==
========================
*/
ID_INLINE bool idImageOpts::operator==( const idImageOpts& opts )
{
	return ( memcmp( this, &opts, sizeof( *this ) ) == 0 );
}

/*
====================================================================

IMAGE

idImage have a one to one correspondance with GL/DX/GCM textures.

No texture is ever used that does not have a corresponding idImage.

====================================================================
*/

static const int	MAX_TEXTURE_LEVELS = 14; // TODO: unused?

// BP: Begin DDS Format support (taken from fhDoom (original D3 fork))

// surface description flags
const unsigned long DDSF_CAPS           = 0x00000001l;
const unsigned long DDSF_HEIGHT         = 0x00000002l;
const unsigned long DDSF_WIDTH          = 0x00000004l;
const unsigned long DDSF_PITCH          = 0x00000008l;
const unsigned long DDSF_PIXELFORMAT    = 0x00001000l;
const unsigned long DDSF_MIPMAPCOUNT    = 0x00020000l;
const unsigned long DDSF_LINEARSIZE     = 0x00080000l;
const unsigned long DDSF_DEPTH          = 0x00800000l;

// pixel format flags
const unsigned long DDSF_ALPHAPIXELS    = 0x00000001l;
const unsigned long DDSF_FOURCC         = 0x00000004l;
const unsigned long DDSF_RGB            = 0x00000040l;
const unsigned long DDSF_RGBA           = 0x00000041l;

// our extended flags
const unsigned long DDSF_ID_INDEXCOLOR	= 0x10000000l;
const unsigned long DDSF_ID_MONOCHROME	= 0x20000000l;

// dwCaps1 flags
const unsigned long DDSF_COMPLEX         = 0x00000008l;
const unsigned long DDSF_TEXTURE         = 0x00001000l;
const unsigned long DDSF_MIPMAP          = 0x00400000l;

#define DDS_MAKEFOURCC(a, b, c, d) ((a) | ((b) << 8) | ((c) << 16) | ((d) << 24))

typedef struct {
    unsigned long dwSize;
    unsigned long dwFlags;
    unsigned long dwFourCC;
    unsigned long dwRGBBitCount;
    unsigned long dwRBitMask;
    unsigned long dwGBitMask;
    unsigned long dwBBitMask;
    unsigned long dwABitMask;
} ddsFilePixelFormat_t;

typedef struct
{
    unsigned long dwSize;
    unsigned long dwFlags;
    unsigned long dwHeight;
    unsigned long dwWidth;
    unsigned long dwPitchOrLinearSize;
    unsigned long dwDepth;
    unsigned long dwMipMapCount;
    unsigned long dwReserved1[11];
    ddsFilePixelFormat_t ddspf;
    unsigned long dwCaps1;
    unsigned long dwCaps2;
    unsigned long dwReserved2[3];
} ddsFileHeader_t;

#ifdef None
#undef None
#endif

enum class pixelFormat_t {
	None,
	RGBA,
	RGBA_32F,
	BGRA,
	RGB,
	BGR,
	DXT1_RGB,
	DXT1_RGBA,
	DXT3_RGBA,
	DXT5_RGBA,
	DXT5_RxGB,
	RGTC,
	DEPTH_24_STENCIL_8,
	DEPTH_24
};

// BP: end of restored DDS support

// How is this texture used?  Determines the storage and color format
typedef enum
{
	TD_SPECULAR,			// may be compressed, and always zeros the alpha channel
	TD_DIFFUSE,				// may be compressed
	TD_DEFAULT,				// generic RGBA texture (particles, etc...)
	TD_BUMP,				// may be compressed with 8 bit lookup
	TD_FONT,				// Font image
	TD_LIGHT,				// Light image
	TD_LOOKUP_TABLE_MONO,	// Mono lookup table (including alpha)
	TD_LOOKUP_TABLE_ALPHA,	// Alpha lookup table with a white color channel
	TD_LOOKUP_TABLE_RGB1,	// RGB lookup table with a solid white alpha
	TD_LOOKUP_TABLE_RGBA,	// RGBA lookup table
	TD_COVERAGE,			// coverage map for fill depth pass when YCoCG is used
	TD_DEPTH,				// depth buffer copy for motion blur
	// RB begin
	TD_SHADOW_ARRAY,		// 2D depth buffer array for shadow mapping
	TD_RGBA16F,
	TD_RGBA32F,
	TD_R32F,
	// RB end
} textureUsage_t;

typedef enum
{
	CF_2D,			// not a cube map
	CF_NATIVE,		// _px, _nx, _py, etc, directly sent to GL
	CF_CAMERA,		// _forward, _back, etc, rotated and flipped as needed before sending to GL
	CF_2D_ARRAY		// not a cube map but not a single 2d texture either
} cubeFiles_t;

enum imageFileType_t
{
	TGA,
	PNG,
	JPG
};

#include "BinaryImage.h"

#define	MAX_IMAGE_NAME	256 // TODO: unused?

class idImage
{
	friend class Framebuffer;
	
public:
	idImage( const char* name );
	~idImage();
	
	const char* 	GetName() const
	{
		return imgName;
	}
	
	// Makes this image active on the current GL texture unit.
	// automatically enables or disables cube mapping
	// May perform file loading if the image was not preloaded.
	void		Bind();
	
	// RB begin
	void		GenerateShadowArray( int width, int height, textureFilter_t filter, textureRepeat_t repeat, textureUsage_t usage );
	// RB end
	
	void		CopyFramebuffer( int x, int y, int width, int height );
	void		CopyDepthbuffer( int x, int y, int width, int height );
	
	void		UploadScratch( const byte* pic, int width, int height );
	
	// estimates size of the GL image based on dimensions and storage type
	int			StorageSize() const;
	
	// print a one line summary of the image
	void		Print() const;
	
	// check for changed timestamp on disk and reload if necessary
	void		Reload( bool force );
	
	void		AddReference()
	{
		refCount++;
	};
	
	void		MakeDefault();	// fill with a grid pattern
	
	const idImageOpts& 	GetOpts() const
	{
		return opts;
	}
	int			GetUploadWidth() const
	{
		return opts.width;
	}
	int			GetUploadHeight() const
	{
		return opts.height;
	}
	
	void		SetReferencedOutsideLevelLoad()
	{
		referencedOutsideLevelLoad = true;
	}
	void		SetReferencedInsideLevelLoad()
	{
		levelLoadReferenced = true;
	}
	void		ActuallyLoadImage( bool fromBackEnd );
	//---------------------------------------------
	// Platform specific implementations
	//---------------------------------------------
	
#if defined( ID_VULKAN )
	void		CreateFromSwapImage( VkImage image, VkImageView imageView, VkFormat format, const VkExtent2D& extent );
	VkImage		GetImage() const
	{
		return image;
	}
	VkImageView	GetView() const
	{
		return view;
	}
	VkImageLayout GetLayout() const
	{
		return layout;
	}
	VkSampler	GetSampler() const
	{
		return sampler;
	}
#endif
	
	void		AllocImage( const idImageOpts& imgOpts, textureFilter_t filter, textureRepeat_t repeat );
	
	// Deletes the texture object, but leaves the structure so it can be reloaded
	// or resized.
	void		PurgeImage();
	
	// z is 0 for 2D textures, 0 - 5 for cube maps, and 0 - uploadDepth for 3D textures. Only
	// one plane at a time of 3D textures can be uploaded. The data is assumed to be correct for
	// the format, either bytes, halfFloats, floats, or DXT compressed. The data is assumed to
	// be in OpenGL RGBA format, the consoles may have to reorganize. pixelPitch is only needed
	// when updating from a source subrect. Width, height, and dest* are always in pixels, so
	// they must be a multiple of four for dxt data.
	void		SubImageUpload( int mipLevel, int destX, int destY, int destZ,
								int width, int height, const void* data,
								int pixelPitch = 0 ) const;
								
	// SetPixel is assumed to be a fast memory write on consoles, degenerating to a
	// SubImageUpload on PCs.  Used to update the page mapping images.
	// We could remove this now, because the consoles don't use the intermediate page mapping
	// textures now that they can pack everything into the virtual page table images.
	void		SetPixel( int mipLevel, int x, int y, const void* data, int dataSize );
	
	// some scratch images are dynamically resized based on the display window size.  This
	// simply purges the image and recreates it if the sizes are different, so it should not be
	// done under any normal circumstances, and probably not at all on consoles.
	void		Resize( int width, int height );
	
	bool		IsCompressed() const
	{
		return ( opts.format == FMT_DXT1 || opts.format == FMT_DXT5 );
	}
	
	
	
	bool		IsLoaded() const
	{
		return texnum != TEXTURE_NOT_LOADED;
	}
	
	static void	GetGeneratedName( idStr& _name, const textureUsage_t& _usage, const cubeFiles_t& _cube );
	
	// used by callback functions to specify the actual data
	// data goes from the bottom to the top line of the image, as OpenGL expects it
	// These perform an implicit Bind() on the current texture unit
	// FIXME: should we implement cinematics this way, instead of with explicit calls?
	void		GenerateImage( const byte* pic,
							   int width, int height,
							   textureFilter_t filter,
							   textureRepeat_t repeat,
							   textureUsage_t usage,
							   int msaaSamples = 0 );
							   
	void		GenerateCubeImage( const byte* pic[6], int size,
								   textureFilter_t filter, textureUsage_t usage );
								   
	void		SetTexParameters();	// update aniso and trilinear
	
private:
	friend class idImageManager;
	
	void		DeriveOpts();
	void		AllocImage();
	void		SetSamplerState( textureFilter_t tf, textureRepeat_t tr );
	
	// parameters that define this image
	idStr				imgName;				// game path, including extension (except for cube maps), may be an image program
	cubeFiles_t			cubeFiles;				// If this is a cube map, and if so, what kind
	void	( *generatorFunction )( idImage* image );	// NULL for files
	textureUsage_t		usage;					// Used to determine the type of compression to use
	idImageOpts			opts;					// Parameters that determine the storage method
	
	// Sampler settings
	textureFilter_t		filter;
	textureRepeat_t		repeat;
	
	bool				referencedOutsideLevelLoad;
	bool				levelLoadReferenced;	// for determining if it needs to be purged
	bool				defaulted;				// true if the default image was generated because a file couldn't be loaded
	ID_TIME_T			sourceFileTime;			// the most recent of all images used in creation, for reloadImages command
	ID_TIME_T			binaryFileTime;			// the time stamp of the binary file
	
	int					refCount;				// overall ref count
	
	static const GLuint TEXTURE_NOT_LOADED = 0xFFFFFFFF;
	
#if defined( ID_VULKAN )
	bool				bIsSwapChainImage;
	VkFormat			internalFormat;
	VkImage				image;
	VkImageView			view;
	VkImageLayout		layout;
	VkSampler			sampler;
	
#if defined( ID_USE_AMD_ALLOCATOR )
	VmaAllocation		allocation;
	static idList< VmaAllocation >		allocationGarbage[ NUM_FRAME_DATA ]; // TODO: NUM_FRAME_DATA?
#else
	vulkanAllocation_t	allocation;
	static idList< vulkanAllocation_t > allocationGarbage[ NUM_FRAME_DATA ];
#endif
	
	static int						garbageIndex;
	static idList< VkImage >		imageGarbage[ NUM_FRAME_DATA ];
	static idList< VkImageView >	viewGarbage[ NUM_FRAME_DATA ];
	static idList< VkSampler >		samplerGarbage[ NUM_FRAME_DATA ];
#else
	GLuint				texnum;				// gl texture binding
	
	// we could derive these in subImageUpload each time if necessary
	GLuint				internalFormat;
	GLuint				dataFormat;
	GLuint				dataType;
#endif
};

// data is RGBA
void	R_WriteTGA( const char* filename, const byte* data, int width, int height, bool flipVertical = false, const char* basePath = "fs_savepath" );
// data is in top-to-bottom raster order unless flipVertical is set

// RB begin
void	R_WritePNG( const char* filename, const byte* data, int bytesPerPixel, int width, int height, bool flipVertical = false, const char* basePath = "fs_savepath" );
// RB end

class idImageManager
{
public:
	idImageManager(idCommon *apCommon, idCmdSystem *apCmdSystem, idFileSystem *apFileSystem, idDeclManager *apDeclManager);
	
	void				Init();
	void				Shutdown();
	
	// If the exact combination of parameters has been asked for already, an existing
	// image will be returned, otherwise a new image will be created.
	// Be careful not to use the same image file with different filter / repeat / etc parameters
	// if possible, because it will cause a second copy to be loaded.
	// If the load fails for any reason, the image will be filled in with the default
	// grid pattern.
	// Will automatically execute image programs if needed.
	idImage* 			ImageFromFile( const char* name,
									   textureFilter_t filter, textureRepeat_t repeat, textureUsage_t usage, cubeFiles_t cubeMap = CF_2D );
									   
	// look for a loaded image, whatever the parameters
	idImage* 			GetImage( const char* name ) const;
	
	// look for a loaded image, whatever the parameters
	idImage* 			GetImageWithParameters( const char* name, textureFilter_t filter, textureRepeat_t repeat, textureUsage_t usage, cubeFiles_t cubeMap ) const;
	
	// The callback will be issued immediately, and later if images are reloaded or vid_restart
	// The callback function should call one of the idImage::Generate* functions to fill in the data
	idImage* 			ImageFromFunction( const char* name, void ( *generatorFunction )( idImage* image ) );
	
	// scratch images are for internal renderer use.  ScratchImage names should always begin with an underscore
	idImage* 			ScratchImage( const char* name, idImageOpts* imgOpts, textureFilter_t filter, textureRepeat_t repeat, textureUsage_t usage );
	
	// purges all the images before a vid_restart
	void				PurgeAllImages();
	
	// reloads all apropriate images after a vid_restart
	void				ReloadImages( bool all );
	
	// Called only by renderSystem::BeginLevelLoad
	void				BeginLevelLoad();
	
	// Called only by renderSystem::EndLevelLoad
	void				EndLevelLoad();
	
	void				Preload( const idPreloadManifest& manifest, const bool& mapPreload );
	
	// Loads unloaded level images
	int					LoadLevelImages( bool pacifier );
	
	void				PrintMemInfo( MemInfo_t* mi );
	
	// built-in images
	void				CreateIntrinsicImages();
	idImage* 			defaultImage;
	idImage* 			flatNormalMap;				// 128 128 255 in all pixels
	idImage* 			alphaNotchImage;			// 2x1 texture with just 1110 and 1111 with point sampling
	idImage* 			whiteImage;					// full of 0xff
	idImage* 			blackImage;					// full of 0x00
	idImage* 			noFalloffImage;				// all 255, but zero clamped
	idImage* 			fogImage;					// increasing alpha is denser fog
	idImage* 			fogEnterImage;				// adjust fogImage alpha based on terminator plane
	// RB begin
	idImage*			shadowImage[5];
	idImage*			jitterImage1;				// shadow jitter
	idImage*			jitterImage4;
	idImage*			jitterImage16;
	idImage*			grainImage1;
	idImage*			randomImage256;
	idImage*			currentRenderHDRImage;
#if defined(USE_HDR_MSAA)
	idImage*			currentRenderHDRImageNoMSAA;
#endif
	idImage*			currentRenderHDRImageQuarter;
	idImage*			currentRenderHDRImage64;
	idImage*			bloomRenderImage[2];
	idImage*			heatmap5Image;
	idImage*			heatmap7Image;
	idImage*			smaaInputImage;
	idImage*			smaaAreaImage;
	idImage*			smaaSearchImage;
	idImage*			smaaEdgesImage;
	idImage*			smaaBlendImage;
	idImage*			currentNormalsImage;			// cheap G-Buffer replacement, holds normals and surface roughness
	idImage*			ambientOcclusionImage[2];		// contain AO and bilateral filtering keys
	idImage*			hierarchicalZbufferImage;		// zbuffer with mip maps to accelerate screen space ray tracing
	// RB end
	idImage* 			scratchImage;
	idImage* 			scratchImage2;
	idImage* 			accumImage;
	idImage* 			currentRenderImage;				// for SS_POST_PROCESS shaders
	idImage* 			currentDepthImage;				// for motion blur
	idImage* 			originalCurrentRenderImage;		// currentRenderImage before any changes for stereo rendering
	idImage* 			loadingIconImage;				// loading icon must exist always
	idImage* 			hellLoadingIconImage;				// loading icon must exist always
	
	//--------------------------------------------------------
	
	idImage* 			AllocImage( const char* name );
	idImage* 			AllocStandaloneImage( const char* name );
	
	bool				ExcludePreloadImage( const char* name );
	
	idList<idImage*, TAG_IDLIB_LIST_IMAGE>	images;
	idHashIndex			imageHash;
	
	bool				insideLevelLoad;			// don't actually load images now
	bool				preloadingMapImages;		// unless this is set
private:
	idCommon *common{nullptr};
	idCmdSystem *cmdSystem{nullptr};
	idFileSystem *fileSystem{nullptr};
	idDeclManager *declManager{nullptr};
};

extern idImageManager*	globalImages;		// pointer to global list for the rest of the system

/*
====================================================================

IMAGEPROCESS

FIXME: make an "imageBlock" type to hold byte*,width,height?
====================================================================
*/

byte* R_Dropsample( const byte* in, int inwidth, int inheight, int outwidth, int outheight );
byte* R_ResampleTexture( const byte* in, int inwidth, int inheight, int outwidth, int outheight );
byte* R_MipMapWithAlphaSpecularity( idCommon *common, const byte* in, int width, int height );
byte* R_MipMapWithGamma( const byte* in, int width, int height );
byte* R_MipMap( const byte* in, int width, int height );

// these operate in-place on the provided pixels
void R_BlendOverTexture( byte* data, int pixelCount, const byte blend[4] );
void R_HorizontalFlip( byte* data, int width, int height );
void R_VerticalFlip( byte* data, int width, int height );
void R_RotatePic( byte* data, int width );
void R_ApplyCubeMapTransforms( int i, byte* data, int size );

/*
====================================================================

IMAGEFILES

====================================================================
*/

void R_LoadImage( const char* name, byte** pic, int* width, int* height, ID_TIME_T* timestamp, bool makePowerOf2, idCommon *apCommon, idFileSystem *apFileSystem );
// pic is in top to bottom raster format
bool R_LoadCubeImages( const char* cname, cubeFiles_t extensions, byte* pic[6], int* size, ID_TIME_T* timestamp, idCommon *apCommon, idFileSystem *apFileSystem );

/*
====================================================================

IMAGEPROGRAM

====================================================================
*/

void R_LoadImageProgram( const char* name, byte** pic, int* width, int* height, ID_TIME_T* timestamp, idCommon *apCommon, idFileSystem *apFileSystem, textureUsage_t* usage = NULL );
const char* R_ParsePastImageProgram( idLexer& src, idCommon *apCommon, idFileSystem *apFileSystem );

