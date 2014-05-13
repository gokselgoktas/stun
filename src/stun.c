/*
 *                     :::::::: ::::::::::: :::    ::: ::::    :::
 *                   :+:    :+:    :+:     :+:    :+: :+:+:   :+:
 *                  +:+           +:+     +:+    +:+ :+:+:+  +:+
 *                 +#++:++#++    +#+     +#+    +:+ +#+ +:+ +#+
 *                       +#+    +#+     +#+    +#+ +#+  +#+#+#
 *               #+#    #+#    #+#     #+#    #+# #+#   #+#+#
 *               ########     ###      ########  ###    ####
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 2.0 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *
 *        http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an AS IS basis,
 * without warranty of any kind, either express or implied.  See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 3 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 3 or later (the "LGPL"), in
 * which case the provisions of the GPL or the LGPL are applicable instead of
 * those above.
 *
 * If you wish to allow use of your version of this file only under the terms
 * of either the GPL or the LGPL, and not to allow others to use your version
 * of this file under the terms of the MPL, indicate your decision by deleting
 * the provisions above and replace them with the notice and other provisions
 * required by the GPL or the LGPL. If you do not delete the provisions above,
 * a recipient may use your version of this file under the terms of any one of
 * the MPL, the GPL or the LGPL. */

#include "stun.h"

/* The number of texture attachments used by the mesh generator.  It
 * corresponds to the number of procedurally generated attribute buffers for
 * a single mesh.
 *
 * Attention: This is an internal value; and every texture has a well-defined
 * purpose.  Hence, modifying this value irresponsibly might lead to extremely
 * serious problems.  Change it carefully. */
#define WZ2X82FB				2

#if !defined (USE_GLEW_FOR_OPENGL_EXTENSIONS) && (FRAMEWORK != GHOST)
	/* This array of pointers holds the function pointers to the OpenGL
	 * extension functions which are dynamically loaded. */
	void *R5UU5MEU[NUMBER_OF_EXTENSIONS];
#endif /* !defined (USE_GLEW_FOR_OPENGL_EXTENSIONS) && (FRAMEWORK != GHOST) */

/* This is the FBO that is used while generating meshes.  It has a certain
 * number textures attached to it.
 *
 * After the mesh generation process, it might be used for other purposes as
 * well.  Such as the G-buffer by logically changing the purpose of the texture
 * attachments, or by adding new attachments if necessary. */
fbo_t HVATQD0L;
texture_t PXDKBCRM[WZ2X82FB]; /* attachments */

/* This is the OpenGL ID for the precompiled default vertex shader.  It simply
 * functions as a pass-through shader and sends up to four UVW-coordinates down
 * the pipeline. */
shader_t MUANXLC;

#if defined (GENERATE_ACCUMULATION_LAYER)
	/* This is the automatically created post-processing layer that is used
	 * as the accumulation layer for the render products. */
	layer_t *P4E6JEEC;
#endif /* defined (GENERATE_ACCUMULATION_LAYER) */

#if (FRAMEWORK != MINIMAL)
	/* If a FRAMEWORK other than MINIMAL is used, then Stun will require
	 * some extra storage space for storing the width and height
	 * information of a possible window. */
	int TVJAYSLH[2];
#endif /* FRAMEWORK != MINIMAL */

/* This is the array of pointers to the null-terminated C-strings that contain
 * unique shader fragments (called stitches) from which we can procedurally
 * generate various shaders. */
static char const *const Z886594B[NUMBER_OF_STITCHES] = {
	"#define x(m) g+=texture2D(f,s m vec2(",
	"0.,float(v)*t",
	"float(v)*t,0.",
	"))*a[14-v]\nuniform sampler2D f;uniform float m,a[15];void main(){vec"
	"2 s=gl_TexCoord[0].xy;float t=1./m;vec4 g=texture2D(f,s)*a[14];for(in"
	"t v=0;v<14;v++)x(+),x(-);gl_FragData[0]=g*g.a;}",
	"void main(){gl_TexCoord[0]=gl_MultiTexCoord0;gl_TexCoord[1]=gl_MultiT"
	"exCoord1;gl_TexCoord[2]=gl_MultiTexCoord2;gl_TexCoord[3]=gl_MultiTexC"
	"oord3;gl_Position = ftransform();}",
	"void main(){vec2 y=gl_FragCoord.xy;gl_FragData[0]=vec4(sin(cos(sin(y."
	"y)*y.x*5.)*y.y*55.)*.5+.5);}",
	"vec4 t(vec4 y){return mod((y*34.+1.)*y,289.);}float r(vec3 y){float v"
	"=1./6.;vec3 d=floor(y+dot(y,vec3(1./3.))),f=y-d+dot(d,vec3(v)),x=step"
	"(f.yzx,f),w=1.-x,a=min(x,w.zxy),z=max(x,w.zxy),p=f-a+v,r=f-z+2.*v,o=f"
	"-1.+3.*v;d=mod(d,289.);vec4 m=t(t(t(d.z+vec4(0.,a.z,z.z,1.))+d.y+vec4"
	"(0.,a.y,z.y,1.))+d.x+vec4(0.,a.x,z.x,1.));v=1./7.;vec4 s=m-49.*floor("
	"m*v*v),e=floor(s*v),c=floor(s-7.*e);e=2.*v*(e-3.25);c=2.*v*(c-3.25);s"
	"=1.-abs(e)-abs(c);m=-step(s,vec4(0.));vec2 l=e.zw;e.zw=c.xy;c.xy=l;e="
	"e.xzyw;c=c.xzyw;e+=(floor(e)*2.+1.)*m.xxyy;c+=(floor(c)*2.+1.)*m.zzww"
	";vec3 n=vec3(e.xy,s.x),u=vec3(e.zw,s.y),b=vec3(c.xy,s.z),i=vec3(c.zw,"
	"s.w);s=1.8-.85*vec4(dot(n,n),dot(u,u),dot(b,b),dot(i,i));vec4 Z=max(."
	"6-vec4(dot(f,f),dot(p,p),dot(r,r),dot(o,o)),0.);return 42.*dot(pow(Z,"
	"vec4(4.)),vec4(dot(n*s.x,f),dot(u*s.y,p),dot(b*s.z,r),dot(i*s.w,o)));"
	"}",
	"uniform float f,o,a;void main(){float y=0.,x=0.;for(;x<o;++x)y+=r(gl_"
	"TexCoord[0].xyz*pow(2.,x)*f)*pow(a,x);gl_FragData[0]=vec4(y)*.5+.5;}",
};

/* And this is the array that stores the final stitched sources before passing
 * everything to glShaderSource(). */
static char const *HH5MTZTK[MAXIMUM_STITCH_SIZE];

/* This array is meant for storing the OpenGL IDs of the GPU programs that are
 * generic and well-defined.  Due to these qualities Stun automates the process
 * of compiling and linking these shaders.
 *
 * Even though the contents of this array can be accessed from elsewhere, it is
 * mean for internal use only. */
program_t FOBIMQQB[ON3400B7];

/* This is the condition for the preprocessor block below. */
#define EVX04ESH (FRAMEWORK == MINIMAL) && (defined (_WIN32) ||		      \
		 (defined (__APPLE__) && defined (__MACH__)))

/* On some platforms, certain dedicated memory management functions depend on
 * memset et al., which are implemented in the CRT (which is not available with
 * the MINIMAL framework).
 *
 * In which case, we'd need to provide our own versions of these functions.
 *
 * However, some other platforms (such as Cocoa) have the ability to provide
 * their own implementations.  Therefor, the conditional takes this into
 * account, also. */
#if (EVX04ESH)
	/* The custom implementation of fill. */
	void fill(void *pointer, int value, unsigned int size)
	{
		register unsigned char *output = pointer;

		while (size-- > 0) {
			*(output++) = (unsigned char) value;
		}
	}

	/* The custom implementation of mirror. */
	void mirror(void const *from, void *to, unsigned int size)
	{
		register unsigned char const *input = from;
		register unsigned char *output = to;

		while (size-- > 0) {
			*(output++) = (*input++);
		}
	}
#endif /* EVX04ESH */

/* This is the first task of the loader.  It simply performs the loading and
 * allocation of fundamental resources, such as OpenGL extensions. */
void ST664M60(void)
{
	int i = 0;
	float *gaussian_weights;

	unsigned char stitches[] = {
		0, 0, 3, END_STITCH_STREAM,	/* Gaussian Blur */
		6, 7, END_STITCH_STREAM,	/* Simplex Noise [generator] */
	};

	J5F2LXAZ(); /* voodoo */

	glGenFramebuffers(1, &HVATQD0L);
	glBindFramebuffer(GL_FRAMEBUFFER, HVATQD0L);

	for (; i < WZ2X82FB; ++i) {
		PXDKBCRM[i] = texture(MAXIMUM_MESH_GRID_WIDTH,
				      MAXIMUM_MESH_GRID_HEIGHT, 0,
				      GL_RGB16F, GL_RGB, GL_FLOAT, NULL);

		glFramebufferTexture2D(GL_FRAMEBUFFER,
				       GL_COLOR_ATTACHMENT0 + i,
				       GL_TEXTURE_2D,
				       PXDKBCRM[i], 0);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	MUANXLC = shader(Z886594B[4], GL_VERTEX_SHADER);

#	if defined (GENERATE_ACCUMULATION_LAYER)
		P4E6JEEC = layer(WINDOW_WIDTH, WINDOW_HEIGHT);
#	endif /* defined (GENERATE_ACCUMULATION_LAYER) */

	gaussian_weights = generate_gaussian_weights(5);

	for (i = 0; i < 2; ++i) {
		stitches[1] = i + 1;

		FOBIMQQB[i] = program(DEFAULT_VERTEX_SHADER, DISCARD,
				      stitch(stitches, GL_FRAGMENT_SHADER));

		glUseProgram(FOBIMQQB[i]);

		glUniform1fv(glGetUniformLocation(FOBIMQQB[i], "a"), 15,
			     gaussian_weights);
	}

	glUseProgram(0);

	FOBIMQQB[2] = pixel_shader(Z886594B[5]);
	FOBIMQQB[3] = program(DEFAULT_VERTEX_SHADER, DISCARD,
			      stitch(&(stitches[4]), GL_FRAGMENT_SHADER));

#	if defined (EXTEND_RESOURCE_MANAGEMENT)
		dispose(gaussian_weights);
#	endif /* defined (EXTEND_RESOURCE_MANAGEMENT) */
}

/* The array of function pointers which point to the loader functions to be
 * called by the loading system. */
loader_t P744X1EL[NUMBER_OF_LOADERS] = {
	ST664M60
};

#if defined (EXTEND_RESOURCE_MANAGEMENT)
	/* This is the first task of the unloader.  Contrastingly to its loader
	 * counterpart it performs the unloading and freeing of fundamental
	 * resources. */
	void TJOZD8F4(void)
	{
		int i = 0;

		glDeleteTextures(WZ2X82FB, PXDKBCRM);
		glDeleteFramebuffers(1, &HVATQD0L);

#		if defined (GENERATE_ACCUMULATION_LAYER)
			glDeleteTextures(1, &(P4E6JEEC->attachment));
			glDeleteFramebuffers(1, &(P4E6JEEC->framebuffer));

			dispose(P4E6JEEC);
#		endif /* defined (GENERATE_ACCUMULATION_LAYER) */

		for (; i < ON3400B7; ++i) {
			glDeleteProgram(FOBIMQQB[i]);
		}
	}

	/* The array of function pointers which point to the resource
	 * deallocation and unloading functions to be called by the loading
	 * system. */
	unloader_t FDC2CHR7[NUMBER_OF_LOADERS] = {
		TJOZD8F4
	};
#endif /* defined (EXTEND_RESOURCE_MANAGEMENT) */

/* shader creates, sets the source code in and compiles a shader.  The shader
 * identifier is returned from the function. */
shader_t shader(char const *source, int type)
{
	shader_t output = glCreateShader(type);

	glShaderSource(output, 1, &source, NULL);
	glCompileShader(output);

	return output;
}

/* The following is the infamous stitcher, by Goksel, made into a proper
 * utility.  It was born with the final version of Youth Uprising's Muon Baryon
 * as a result of fifteen minutes of hacking, then it was re-abused by Jake in
 * Magnus and apparently also in Ethos.
 *
 * What it does is to maintain a list of pointers to the individual AND unique
 * shader fragments (stitches).  One can then use these stitches to stitch a
 * rope of shader strings.
 *
 * And this function is the entire brain of the stitching utility.  It accepts
 * an array of integers (possibly 8-bit too) which are indices of the shader
 * stitches in the internally maintained array of the stitcher.  A special
 * marker (END_STITCH_STREAM) indicates the end of the indices stream.
 *
 * It then constructs an array of pointers to the strings so that the fragments
 * are laid out in a way when interpreted linearly they construct the desired
 * shader source.  Finally it passes this array to glShaderSource and let's the
 * action begin.
 *
 * Any shader stitch can be used for any shader as long as the said shader
 * supports all the functions utilized (as in no texture2D in vertex or
 * geometry shaders).
 *
 * The second parameter of this function is the type of the shader.  It can be
 * GL_FRAGMENT_SHADER, GL_GEOMETRY_SHADER or GL_VERTEX_SHADER (or well,
 * anything else that OpenGL supports).
 *
 * In the end, this function returns the OpenGL shader-id of the shader after
 * compiling it. */
shader_t stitch(unsigned char const *indices, int type)
{
	shader_t output;

	unsigned char index = 0;

	for (; *indices != END_STITCH_STREAM; ++indices, ++index) {
		HH5MTZTK[index] = Z886594B[*indices];
	}

	output = glCreateShader(type);

	glShaderSource(output, index, HH5MTZTK, NULL);
	glCompileShader(output);

	return output;
}

/* program creates a program to which it attaches the given vertex, fragment
 * and geometry shaders.  Afterwards it returns the program id after linking
 * it.
 *
 * If you don't want to have any of the three shaders, simply set the
 * identifier to DISCARD.  However, at least a fragment shader is necessary. */
program_t program(shader_t vertex, shader_t geometry, shader_t fragment)
{
	program_t output = glCreateProgram();

	if (geometry != DISCARD)
		glAttachShader(output, geometry);

	if (vertex != DISCARD)
		glAttachShader(output, vertex);

	glAttachShader(output, fragment);
	glLinkProgram(output);

	return output;
}

/* The texture function allocates an OpenGL name for a texture object as well
 * as the video memory requested by the texture. */
texture_t texture(unsigned int width, unsigned int height, int level,
		  int internalFormat, int format, int type, void *data)
{
	texture_t output;

	glGenTextures(1, &output);
	glBindTexture(GL_TEXTURE_2D, output);

	glTexImage2D(GL_TEXTURE_2D, level, internalFormat, width, height, 0,
		     format, type, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	return output;
}

/* The layer function creates a post-processing layer; essentially an FBO with
 * a predefined texture of the specified size attached to it.  In contrast,
 * layers don't sport a fancy depth buffer. */
layer_t *layer(unsigned int width, unsigned int height)
{
	layer_t *output = allocate(sizeof(layer_t));

	output->width = width;
	output->height = height;

	glGenFramebuffers(1, &(output->framebuffer));
	glBindFramebuffer(GL_FRAMEBUFFER, output->framebuffer);

	output->attachment = texture(width, height, 0, GL_RGBA16F, GL_RGBA,
				     GL_FLOAT, NULL);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			       GL_TEXTURE_2D, output->attachment, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return output;
}

/* The depth function provides a special kind of a layer; an FBO with a
 * predefined texture of the specified size attached to it.  But, differently
 * from the layer function the texture is a depth attachment. */
layer_t *depth(unsigned int width, unsigned int height)
{
	layer_t *output = allocate(sizeof(layer_t));

	output->width = width;
	output->height = height;

	glGenFramebuffers(1, &(output->framebuffer));
	glBindFramebuffer(GL_FRAMEBUFFER, output->framebuffer);

	output->attachment = texture(width, height, 0, GL_DEPTH_COMPONENT,
				     GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
			       GL_TEXTURE_2D, output->attachment, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return output;
}

/* The mesh function is the implementation of a crazy idea that does hopefully
 * work without causing any harm.
 *
 * The idea is to stream-out the GPU calculated 3D meshes onto the RAM.  This
 * is achieved by laying out quads on an orthographically projected canvas in
 * such a way that every vertex of each quad gets to be drawn but the quads
 * that are not on the edges consist entirely of shared vertices.
 *
 * Which means, logically, our mesh has a continuous initial surface; and the
 * modelling GPU program can take advantage of this.
 *
 * This layout also renders it possible to actually modify the geometry within
 * the fragment shader at a per-vertex level, too.
 *
 * Then a GPU program is ran over this grid.  The key point of the GPU program
 * is that it doesn't modify the actual geometry but rather stores the
 * transformation of each vertex of each quad at its corresponding pixel on the
 * canvas (= framebuffer).
 *
 * On top of that the GPU program also calculates and stores the texturing
 * coordinates of the geometry and stores them into a separate render-target.
 *
 * Finally these values are read back to the RAM using 'glReadPixels' and are
 * re-ordered.  Additionally, if Stun is configured to do so, some other
 * features are also automatically calculated.
 *
 * This is all, after this your GPU modelled mesh is ready to be rendered in
 * realtime with no further calculations required! */
mesh_t *mesh(program_t gpu_program, unsigned int width, unsigned int height)
{
	/* This is the width of the generation canvas we use.
	 *
	 * It's one pixel larger than the requested width, because the last
	 * quad on each row requires this for its right-side vertices. */
	unsigned int const x_span = width + 1;

	/* This is the height of the generation canvas.
	 *
	 * Again the reason it is one pixel larger than the requested height,
	 * because the last quad on each column requires this for its bottom
	 * vertices. */
	unsigned int const y_span = height + 1;

	/* There are exactly two faces (= triangles) per quad.  Hence the
	 * number of faces is the same as two times the product of width and
	 * height. */
	unsigned int const number_of_faces = (width * height) << 1;

	/* Every quad has four vertices and unless they are the last or the
	 * first quads in a row or a column then all of their vertices are
	 * shared.
	 *
	 * This means we need to address the unshared vertices at each column
	 * and at each row. */
	unsigned int const number_of_vertices = x_span * y_span;

	/* These are the color attachments of our FBO to which we will store
	 * our geometry products. */
	unsigned int draw_buffers[] = {
		GL_COLOR_ATTACHMENT0, /* Vertex positions. */
		GL_COLOR_ATTACHMENT1  /* UVW coordinates. */
	};

	/* The following two are used as counters for the for-loop that lays
	 * out the quad grid. */
	unsigned int x = 0;
	unsigned int y;

	/* And the following one is used as the current index count, every quad
	 * has six vertices hence this increases by six per quad. */
	unsigned int index = 0;

	/* We start off by allocating memory for our mesh. */
	mesh_t *mesh = allocate(sizeof(mesh_t));

	/* The three there is because every single vertex has three components.
	 * And each component is a float. */
	mesh->vertices = allocate(number_of_vertices * 3 * sizeof(float));

	/* Same goes for the normals.  Every vertex has a normal vector which
	 * is a three-component float. */
	mesh->normals = allocate(number_of_vertices * 3 * sizeof(float));

	/* And, in companion to the normal, every vertex also features a
	 * tangent vector.  Just like a normal vector, it is also a
	 * three-component float. */
	mesh->tangents = allocate(number_of_vertices * 3 * sizeof(float));

	/* Here comes the UVW coordinates.  These are meant for texturing and
	 * each vertex has three UVW coordinates. */
	mesh->uvw_coordinates =
		allocate(number_of_vertices * 3 * sizeof(float));

	/* Finally the indices.  Each indice specifies the index of the three
	 * vertices that make up a face.  So there are number of faces times
	 * three indices in total. */
	mesh->indices =
		allocate(number_of_faces * 3 * sizeof(unsigned int));

	/* We store the number of faces our mesh has. */
	mesh->number_of_faces = number_of_faces;

	/* And of course the number of vertex attributes it has. */
	mesh->number_of_vertices = number_of_vertices;

	/* - - - - - - - - - - - - - - - - 8< - - - - - - - - - - - - - - - */

	/* We're done with allocating space for the mesh.  Next up is to
	 * actually generate it! */

	/* Store the current OpenGL viewport values.  We don't want to mess
	 * with them for good reasons. */
	glPushAttrib(GL_VIEWPORT_BIT);

	/* Go go, action bronco.  We bind our (hopefully, successfully created)
	 * FBO that contains 16-bit floating-point render targets. */
	glBindFramebuffer(GL_FRAMEBUFFER, HVATQD0L);

	/* Here we specify to which color attachments of our FBO we'll be
	 * writing to. */
	glDrawBuffers(2, draw_buffers);

	/* Set the orthographic projection.  This ensures that every vertex of
	 * every quad maps onto a pixel on the screen.
	 *
	 * Note that it also changes the viewport accordingly. */
	orthographic(x_span, y_span);

	/* Finally activate the modelling GPU program and let the fun kick. */
	glUseProgram(gpu_program);

	/* Here we lay our quad grid on our canvas.  Basically all we do is to
	 * lay them in a way that will construct one huge full-viewport
	 * rectangle with no holes or gaps.
	 *
	 * This ensures the continuity of the surface of our mesh when the GPU
	 * program starts modelling it.  It's free too! */
	for (; x < width; ++x) {
		for (y = 0; y < height; ++y) {
			/* Here we calculate the output index of our current
			 * triangle's upper-left vertex.
			 *
			 * We can assure this because the work-flow of 'glRect'
			 * is very well defined in the OpenGL Specification.
			 *
			 * We use this index to procedurally fill our mesh's
			 * index array. */
			unsigned int const vertex = x + y * x_span;

			/* Yay, we put a happy quad on our grid. */
			glRecti(x, y, x + 2, y + 2);

			/* And finally we also fill in our indices to the
			 * mesh's index array.  Thanks to 'glRect' being so
			 * well defined we can actually procedurally generate
			 * these!
			 *
			 * The order in which we draw (and hence index) our
			 * triangles is the same order that 'glRect' uses:
			 *
			 * 0------->-------1  As the shape to the left shows,
			 * |             / |  this is the order in which we
			 * |           /   |  index our triangles:
			 * |         /     |
			 * |      > <      v  0 - 1 - 2 for the first face &
			 * |     /         |  1 - 3 - 2 for the second face.
			 * |   /           |
			 * | /             |
			 * 2-------<-------3 */

			/* This is the top-left vertex of our quad and of our
			 * first triangle. */
			mesh->indices[index + 0] = vertex;

			/* And this is the top-right vertex of the quad and of
			 * our triangle. */
			mesh->indices[index + 1] = vertex + 1;

			/* Now we finish off the first triangle with the
			 * bottom-left vertex of the quad. */
			mesh->indices[index + 2] = vertex + x_span;

			/* And this is the top-right vertex of the quad, also
			 * the apex of our second triangle. */
			mesh->indices[index + 3] = vertex + 1;

			/* Now the bottom-left vertex of the quad again… */
			mesh->indices[index + 4] = vertex + x_span + 1;

			/* And the last vertex that forms our second triangle
			 * and of course this also finalizes our quad. */
			mesh->indices[index + 5] = vertex + x_span;

			/* On to the next quad! */
			index += 6;
		}
	}

	/* Disable the GPU program.  Forgetting this would certainly lead to
	 * disasters! */
	glUseProgram(0);

	/* - - - - - - - - - - - - - - - - 8< - - - - - - - - - - - - - - - - */

	/* This is the entire trick!  We read back from our off-screen
	 * framebuffer textures into our mesh structure. */

	/* First and foremost of all, the vertex positions. */
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glReadPixels(0, 0, x_span, y_span, GL_RGB, GL_FLOAT, mesh->vertices);

	/* And then, the UVW coordinates. */
	glReadBuffer(GL_COLOR_ATTACHMENT1);
	glReadPixels(0, 0, x_span, y_span, GL_RGB, GL_FLOAT,
		     mesh->uvw_coordinates);


	/* Make sure we restore back the framebuffer too.  Otherwise, we might
	 * end up with a crying face! */
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	/* Restore back the original viewport. */
	glPopAttrib();

	/* Next, flush the OpenGL pipeline; so, everything up until here gets
	 * processed.
	 *
	 * It's possible that, we'll be needing the procedural data from this
	 * step in order to generate the normals and the tangent-bases.  Hence,
	 * forgetting to flush the OpenGL pipeline might lead to the corruption
	 * of normal and tangent buffers. */
	glFinish();

	/* - - - - - - - - - - - - - - - [ + ] - - - - - - - - - - - - - - - */

	/* Stun can be configured to calculate certain vertex attributes of a
	 * mesh (such as the tangent-space normals and the tangent vectors).
	 *
	 * This is the place where the actual calculation takes place if such
	 * a request is made. */
#	if defined (COMPUTE_VERTEX_ATTRIBUTES)

		/* The code snippet that used to perform this task on request,
		 * has been refactored in order to achieve a more flexible
		 * solution.
		 *
		 * This newly spun-off function, compute_vertex_attributes, can
		 * be found below. */
		compute_vertex_attributes(mesh);

#	endif /* defined (COMPUTE_VERTEX_ATTRIBUTES) */

	/* - - - - - - - - - - - - - - - [ + ] - - - - - - - - - - - - - - - */

	/* Return the newly created mesh.  And this is it babe. */
	return mesh;
}

/* Given a mesh, the compute_vertex_attributes function generates certain
 * vertex attributes (such as the vertex normals and the vertex tangents) using
 * the topology of the provided mesh.
 *
 * These newly generated values are then stored in the corresponding buffers of
 * the specified mesh. */
void compute_vertex_attributes(mesh_t *mesh)
{
	unsigned int i = 0;

	/* We start off by filling all of the vertex attribute buffers with
	 * zeros.
	 *
	 * This step is quite necessary because we'll be accumulating values in
	 * these buffers in the next step.  We, surely, wouldn't want to
	 * accumulate over garbage memory. */
	unsigned int const size = mesh->number_of_vertices * 3 * sizeof(float);

	/* These buffers will be used for value accumulation.  Thus, they need
	 * to be filled with zeroes in order to avoid surprises. */
	zero(mesh->normals, size);
	zero(mesh->tangents, size);

	/* Then we move onto the actual calculations of vertex attributes.  For
	 * now, two components of the TBN matrix: the normal and the tangent
	 * vectors are calculated.
	 *
	 * The bitangent vector can be derived in a shader using the
	 * cross-product of the aforementioned two. */
	for (i = 0; i < mesh->number_of_faces; ++i) {
		unsigned int j = 0;

		/* The indices of the three vertices that make up the current
		 * face we are processing. */
		unsigned int const triangle[3] = {
			mesh->indices[i * 3 + 0] * 3,
			mesh->indices[i * 3 + 1] * 3,
			mesh->indices[i * 3 + 2] * 3
		};

		/* We pick the second vertex of our triangle as the reference
		 * point according to which we'll calculate the surface
		 * attributes.
		 *
		 * Then we construct two vectors from this vertex to the two
		 * other vertices of the same face (= triangle).
		 *
		 * We perform these calculations not only for the positions of
		 * these vertices but also for their UV coordinates.  The UV
		 * deltas are needed for calculating the tangent vector. */
		float const edges[8] = {
			/* The three components below form a vector from the
			 * second vertex to the first vertex.  This vector
			 * represents the first edge of our triangle. */
			mesh->vertices[triangle[0] + 0] -
			mesh->vertices[triangle[1] + 0],

			mesh->vertices[triangle[0] + 1] -
			mesh->vertices[triangle[1] + 1],

			mesh->vertices[triangle[0] + 2] -
			mesh->vertices[triangle[1] + 2],

			/* The three components below form a vector from the
			 * second vertex to the third vertex.  This vector
			 * represents the second edge of our triangle. */
			mesh->vertices[triangle[2] + 0] -
			mesh->vertices[triangle[1] + 0],

			mesh->vertices[triangle[2] + 1] -
			mesh->vertices[triangle[1] + 1],

			mesh->vertices[triangle[2] + 2] -
			mesh->vertices[triangle[1] + 2],

			/* The two components below form a vector from the UV
			 * coordinates of the second vertex to those of the
			 * first one.  This vector represents the first UV
			 * delta of our triangle's vertices.
			 *
			 * However, the first one of these two components is
			 * commented out as it isn't used for anything.
			 *
			 * - - - - - - - - - - - - - - - - - - - - - - - - - -
			 *
			 * mesh->uvw_coordinates[indices[0] + 0] -
			 * mesh->uvw_coordinates[indices[1] + 0], */

			mesh->uvw_coordinates[triangle[0] + 1] -
			mesh->uvw_coordinates[triangle[1] + 1],

			/* The two components below form a vector from the UV
			 * coordinates of the second vertex to those of the
			 * third one.  This vector represents the second UV
			 * delta of our triangle's vertices.
			 *
			 * However, the first one of these two components is
			 * commented out as it isn't used for anything.
			 *
			 * - - - - - - - - - - - - - - - - - - - - - - - - - -
			 *
			 * mesh->uvw_coordinates[indices[2] + 0] -
			 * mesh->uvw_coordinates[indices[1] + 0], */

			mesh->uvw_coordinates[triangle[2] + 1] -
			mesh->uvw_coordinates[triangle[1] + 1]
		};

		/* The non-unit normal vector can be calculated by taking the
		 * cross-product of the two edges from above. */
		float const normal[3] = {
			edges[1] * edges[5] - edges[4] * edges[2],
			edges[2] * edges[3] - edges[5] * edges[0],
			edges[0] * edges[4] - edges[3] * edges[1],
		};

		/* And, the non-unit tangent vector can be calculated by
		 * scaling the two edges according to the T and B counterparts
		 * that come from the UV deltas. */
		float const tangent[3] = {
			edges[3] * edges[6] - edges[0] * edges[7],
			edges[4] * edges[6] - edges[1] * edges[7],
			edges[5] * edges[6] - edges[2] * edges[7],
		};

		/* Then, for each vertex we accumulate the normal and tangent
		 * vectors from each one of the faces it belongs to.
		 *
		 * This way, we can generate smooth face normals per vertex;
		 * in other words: vertex normals. */
		for (; j < 3; ++j) {
			mesh->normals[triangle[j] + 0] += normal[0];
			mesh->normals[triangle[j] + 1] += normal[1];
			mesh->normals[triangle[j] + 2] += normal[2];

			mesh->tangents[triangle[j] + 0] += tangent[0];
			mesh->tangents[triangle[j] + 1] += tangent[1];
			mesh->tangents[triangle[j] + 2] += tangent[2];
		}
	}

	/* Finally, we go through all of the vertices of the mesh, and
	 * normalize the accumulated normal and tangent vectors to obtain unit
	 * length vectors. */
	for (i = 0; i < mesh->number_of_vertices; ++i) {
		float magnitude = sqrtf(square(mesh->normals[i * 3 + 0]) +
					square(mesh->normals[i * 3 + 1]) +
					square(mesh->normals[i * 3 + 2]));

		if (magnitude != 0.f) {
			mesh->normals[i * 3 + 0] /= magnitude;
			mesh->normals[i * 3 + 1] /= magnitude;
			mesh->normals[i * 3 + 2] /= magnitude;
		}

		magnitude = sqrtf(square(mesh->tangents[i * 3 + 0]) +
				  square(mesh->tangents[i * 3 + 1]) +
				  square(mesh->tangents[i * 3 + 2]));

		if (magnitude != 0.f) {
			mesh->tangents[i * 3 + 0] /= magnitude;
			mesh->tangents[i * 3 + 1] /= magnitude;
			mesh->tangents[i * 3 + 2] /= magnitude;
		}
	}
}

/* Given a mesh and a tolerance value, this function consolidates the given
 * mesh by removing any logical duplicates of a vertex.
 *
 * A vertex is marked as a logical duplicate if it is within a certain radius
 * of another vertex; the radius is specified by the tolerance value.
 *
 * The corresponding buffers of the given mesh are then replaced by these newly
 * modified buffers. */
void consolidate(mesh_t *mesh, float tolerance)
{
	unsigned int j;
	unsigned int k;

	unsigned int i = 0;

	/* This buffer will be used to collect unique vertices from the current
	 * vertex buffer of the given mesh. */
	float *vertices =
		allocate(mesh->number_of_vertices * 3 * sizeof(float));

	/* This buffer will be used to collect the normals of the unique
	 * vertices from the current vertex normal buffer of the mesh. */
	float *normals =
		allocate(mesh->number_of_vertices * 3 * sizeof(float));

	/* This buffer will be used to collect the tangents of the unique
	 * vertices from the current vertex tangent buffer of the mesh. */
	float *tangents =
		allocate(mesh->number_of_vertices * 3 * sizeof(float));

	/* This buffer will be used to collect the UVW coordinates of the
	 * unique vertices from the current UVW coordinates buffer of the
	 * mesh. */
	float *uvw_coordinates =
		allocate(mesh->number_of_vertices * 3 * sizeof(float));

	/* This buffer will be used to store the modified triangle indices of
	 * the faces of the mesh. */
	unsigned int *indices =
		allocate(mesh->number_of_faces * 3 * sizeof(unsigned int));

	/* The new number of vertices after removing the duplicates. */
	unsigned int number_of_vertices = 0;

	for (; i < mesh->number_of_faces; ++i) {
		/* The indices of the three vertices that make up the current
		 * face we are processing. */
		unsigned int const triangle[3] = {
			mesh->indices[i * 3 + 0] * 3,
			mesh->indices[i * 3 + 1] * 3,
			mesh->indices[i * 3 + 2] * 3
		};

		/* Every face has three vertices, and we'll be repeating the
		 * same actions for each one of these vertices. */
		for (j = 0; j < 3; ++j) {

			/* Go through all of the unique vertices, and compare
			 * the current vertex against each one of these unique
			 * vertices. */
			for (k = 0; k < number_of_vertices; ++k) {
				unsigned int const index = k * 3;

				/* Calculate a vector from the current unique
				 * vertex to the vertex of the mesh we are
				 * checking for. */
				float difference[3] = {
					mesh->vertices[triangle[j] + 0] -
					vertices[index + 0],

					mesh->vertices[triangle[j] + 1] -
					vertices[index + 1],

					mesh->vertices[triangle[j] + 2] -
					vertices[index + 2]
				};

				/* Compute the square of the distance between
				 * the two vertices.
				 *
				 * We use the squared radius instead of the
				 * radius to save ourselves an sqrtf call. */
				float const radius_squared =
					square(difference[0]) +
					square(difference[1]) +
					square(difference[2]);

				/* Check if it is within the given radius of
				 * a unique vertex. */
				if (radius_squared < square(tolerance)) {
					break;
				}
			}

			/* If the vertex from the mesh is not found within the
			 * unique vertices, it means it is yet another unique
			 * vertex. */
			if (k == number_of_vertices) {
				unsigned int const index =
					number_of_vertices * 3;

				/* The following three assignments add the
				 * vertex to the unique vertices buffer. */
				vertices[index + 0] =
					mesh->vertices[triangle[j] + 0];

				vertices[index + 1] =
					mesh->vertices[triangle[j] + 1];

				vertices[index + 2] =
					mesh->vertices[triangle[j] + 2];

				/* The following three assignments add the
				 * vertex normal to the unique vertex normals
				 * buffer. */
				normals[index + 0] =
					mesh->normals[triangle[j] + 0];

				normals[index + 1] =
					mesh->normals[triangle[j] + 1];

				normals[index + 2] =
					mesh->normals[triangle[j] + 2];

				/* The following three assignments add the
				 * per-vertex tangent to the unique tangents
				 * buffer. */
				tangents[index + 0] =
					mesh->tangents[triangle[j] + 0];

				tangents[index + 1] =
					mesh->tangents[triangle[j] + 1];

				tangents[index + 2] =
					mesh->tangents[triangle[j] + 2];

				/* The following three assignments add the
				 * UVW coordinates to the unique UVW
				 * coordinates buffer. */
				uvw_coordinates[index + 0] =
					mesh->uvw_coordinates[triangle[j] + 0];

				uvw_coordinates[index + 1] =
					mesh->uvw_coordinates[triangle[j] + 1];

				uvw_coordinates[index + 2] =
					mesh->uvw_coordinates[triangle[j] + 2];

				++number_of_vertices;
			}

			/* Adjust the index of the current vertex according to
			 * the result from above. */
			indices[i * 3 + j] = k;
		}
	}

	/* Finally, we replace the vertex and index buffers of our mesh with
	 * the new ones that we just generated. */
	dispose(mesh->vertices);
	dispose(mesh->normals);
	dispose(mesh->tangents);
	dispose(mesh->uvw_coordinates);
	dispose(mesh->indices);

	mesh->number_of_vertices = number_of_vertices;

	mesh->vertices = vertices;
	mesh->normals = normals;
	mesh->tangents = tangents;
	mesh->uvw_coordinates = uvw_coordinates;
	mesh->indices = indices;
}

/* Given a mesh, this function simply asks OpenGL to render it.  Note that it
 * doesn't perform any translations, rotations or scaling. */
void render(mesh_t const *mesh)
{
	glVertexPointer(3, GL_FLOAT, 0, mesh->vertices);
	glNormalPointer(GL_FLOAT, 0, mesh->normals);
	glColorPointer(3, GL_FLOAT, 0, mesh->tangents);
	glTexCoordPointer(3, GL_FLOAT, 0, mesh->uvw_coordinates);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glDrawElements(GL_TRIANGLES, mesh->number_of_faces * 3,
		       GL_UNSIGNED_INT, mesh->indices);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

/* perspective sets up a perspective projection with the given width, height
 * and aspect ratio.  It also resizes the viewport accordingly. */
void perspective(unsigned int width, unsigned int height, float aspect_ratio)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/* This call mimics a gluPerspective call with the following arguments:
	 *
	 *        field-of-view angle	: 45°
	 *        aspect ratio		: 'aspect_ratio'
	 *        near			: 1.
	 *        far			: 100. */
	glFrustum(-FRUSTUM * aspect_ratio, +FRUSTUM * aspect_ratio, -FRUSTUM,
		  +FRUSTUM, NEAR_PLANE_DEPTH, FAR_PLANE_DEPTH);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/* orthographic sets up a two-dimensional orthographic projection with the
 * given width and height.  Again, it also resizes the viewport accordingly. */
void orthographic(unsigned int width, unsigned int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0., (double) width, (double) height, 0., -1., 1.);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/* The is_interval_hit function returns true every n seconds where n is the
 * specified amount of seconds.
 *
 * It also snapshots the current time and stores it in timer; guaranteeing
 * continuous action. */
int is_interval_hit(float *timer, float interval)
{
	if (since(*timer) >= interval) {
		*timer = seconds();
		return true;
	}

	return false;
}

/* The quad function draws a unit-textured quad of the specified width and
 * height. */
void quad(float width, float height)
{
	glBegin(GL_QUADS);
		glTexCoord2s(0, 1); glVertex2f(0.f, 0.f);
		glTexCoord2s(1, 1); glVertex2f(width, 0.f);
		glTexCoord2s(1, 0); glVertex2f(width, height);
		glTexCoord2s(0, 0); glVertex2f(0.f, height);
	glEnd();
}

/* far_plane renders a 2D orthographic quad whose texturing coordinates
 * correspond to the view-space x and y values of the frustum's far-plane.
 *
 * The interpolated texturing coordinates would be amazingly useful for
 * constructing a camera ray from the eye point towards the far-plane passing
 * through each and every pixel on the screen. */
void far_plane(unsigned short int width, unsigned short int height)
{
	orthographic(width, height);

	glBegin(GL_QUADS);
		glTexCoord2f(-19.882f, +12.426f); glVertex2s(0, 0);
		glTexCoord2f(+19.882f, +12.426f); glVertex2s(width, 0);
		glTexCoord2f(+19.882f, -12.426f); glVertex2s(width, height);
		glTexCoord2f(-19.882f, -12.426f); glVertex2s(0, height);
	glEnd();
}

/* The generate function generates the elements of the given destination layer
 * using the output from the supplied GPU program. */
void generate(layer_t *destination, program_t generator)
{
	orthographic(destination->width, destination->height);

	glBindFramebuffer(GL_FRAMEBUFFER, destination->framebuffer);
	glUseProgram(generator);

	quad((float) destination->width, (float) destination->height);

	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/* The transform function, as its name suggests, transforms the elements of the
 * given source layer via the supplied GPU program and then writes the newly
 * computed render products into the destination layer. */
void transform(layer_t *source, layer_t *destination, program_t transformer)
{
	orthographic(destination->width, destination->height);

	glBindFramebuffer(GL_FRAMEBUFFER, destination->framebuffer);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, source->attachment);

	glUseProgram(transformer);

	quad((float) destination->width, (float) destination->height);

	glUseProgram(0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/* The blit function simply blits the elements of the source layer onto the
 * destination layer using the fixed-function pipeline.
 *
 * If the sizes of the layers involved are different then the source layer is
 * linearly scaled to fit the size of the destination layer. */
void blit(layer_t *source, layer_t *destination)
{
	orthographic(destination->width, destination->height);

	glBindFramebuffer(GL_FRAMEBUFFER, destination->framebuffer);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, source->attachment);

	glEnable(GL_TEXTURE_2D);

	quad((float) destination->width, (float) destination->height);

	glDisable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/* The blit_to_back_buffer function is a specialization of the functionality
 * provided by the blit function.
 *
 * Given a source layer, this function blits the contents of the layer to the
 * back-buffer, scaling the texture to cover the entire window. */
void blit_to_back_buffer(layer_t *source)
{
	orthographic(WINDOW_WIDTH, WINDOW_HEIGHT);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, source->attachment);

	glEnable(GL_TEXTURE_2D);

	quad((float) WINDOW_WIDTH, (float) WINDOW_HEIGHT);

	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

#if defined (ENABLE_MATHEMATICS_SUPPORT)
	/* This function, as its name suggests, generates an array of Gaussian
	 * weights.  This array can be extremely useful for doing things like
	 * Gaussian-blur. */
	float *generate_gaussian_weights(float standard_deviation)
	{
		int const width = (int) (3.f * standard_deviation) - 1;
		int const size = (width << 1) + 1;

		float sum = 0.f;
		float *output = allocate(size * sizeof(float));

		int i = 0;
		for (; i < size; ++i) {
			output[i] = gaussian((float) (i - width),
					     standard_deviation);
			sum += output[i];
		}

		for (i = 0; i < size; ++i) {
			output[i] /= sum;
		}

		return output;
	}

	/* With the MINIMAL framework, Stun assumes that the floating-point
	 * model is 'fast'; which means, the intrinsics are available. */
#	if (FRAMEWORK == MINIMAL)
		/* However, on Visual C++, without the CRT, the compiler can't
		 * link against its intrinsics.  But, of course, Stun can
		 * exploit this to our benefit. */
#		if defined (_MSC_VER)
			/* _CIsqrt calculates the square root of the given,
			 * positive (including zero), floating-point value. */
			__declspec (naked) float _CIsqrt()
			{
				__asm { fsqrt
					ret };
			}

			/* _CIexp calculates the exponential function for the
			 * given floating-point value. */
			__declspec (naked) float _CIexp()
			{
				__asm { fldl2e
					fmulp st(1), st(0)
					fld1
					fld st(1)
					fprem
					f2xm1
					faddp st(1), st(0)
					fscale
					fstp st(1)
					ret };
			}

			/* _CIsin calculates the sine of the given value; which
			 * is an angle (in radians). */
			__declspec (naked) float _CIsin()
			{
				__asm { fsin
					ret };
			}

			/* _CIcos calculates the cosine of the given value;
			 * which is an angle (in radians). */
			__declspec (naked) float _CIcos()
			{
				__asm { fcos
					ret };
			}
#		endif /* defined (_MSC_VER) */
#	endif /* FRAMEWORK == MINIMAL */
#endif /* defined (ENABLE_MATHEMATICS_SUPPORT) */
