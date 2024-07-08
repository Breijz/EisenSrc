project('EisenSrc', ['c', 'cpp'])

GLAD_INC = include_directories('include/glad')
QWSV_INC = include_directories('include/mp')
QWCL_INC = include_directories('include/sp')
QWCOMMON_INC = include_directories('include/common')

GLSL_INC = include_directories('shaders')

UTIL_PAK_INC = include_directories('include/util')

GLAD_SRC = files('src/sp/glad.c')

QWSV_SRC = files(
  'src/mp/pr_cmds.c',
  'src/mp/pr_edict.c',
  'src/mp/pr_exec.c',
  'src/mp/server/sv_init.c',
  'src/mp/server/sv_main.c',
  'src/mp/server/sv_nchan.c',
  'src/mp/server/sv_ents.c',
  'src/mp/server/sv_send.c',
  'src/mp/server/sv_move.c',
  'src/mp/server/sv_phys.c',
  'src/mp/server/sv_user.c',
  'src/mp/server/sv_ccmds.c',
  'src/mp/world.c',
  'src/mp/system/sys_unix.c',
  'src/mp/server/sv_model.c',
  'src/sp/cmd.c',
  'src/sp/common.c',
  'src/sp/crc.c',
  'src/sp/cvar.c',
  'src/sp/mathlib.c',
  'src/sp/md4.c',
  'src/sp/zone.c',
  'src/sp/pmove.c',
  'src/sp/pmovetst.c',
  'src/sp/net/net_chan.c',
  'src/sp/net/net_udp.c'
)

QWCL_SRC = files(
  'src/sp/client/cl_demo.c',
  'src/sp/client/cl_ents.c',
  'src/sp/client/cl_input.c',
  'src/sp/client/cl_main.c',
  'src/sp/client/cl_parse.c',
  'src/sp/client/cl_pred.c',
  'src/sp/client/cl_tent.c',
  'src/sp/client/cl_cam.c',
  'src/sp/cmd.c',
  'src/sp/common.c',
  'src/sp/console.c',
  'src/sp/crc.c',
  'src/sp/cvar.c',
  'src/sp/keys.c',
  'src/sp/mathlib.c',
  'src/sp/md4.c',
  'src/sp/menu.c',
  'src/sp/net/net_chan.c',
  'src/sp/net/net_udp.c',
  'src/sp/nonintel.c',
  'src/sp/pmove.c',
  'src/sp/pmovetst.c',
  'src/sp/r_part.c',
  'src/sp/sbar.c',
  'src/sp/skin.c',
  'src/sp/sound/snd_dma.c',
  'src/sp/sound/snd_mem.c',
  'src/sp/sound/snd_mix.c',
  'src/sp/view.c',
  'src/sp/wad.c',
  'src/sp/zone.c',
  'src/sp/cd/cd_linux.c',
  'src/sp/system/sys_linux.c',
  'src/sp/sound/snd_linux.c',
)

OGL_1_1_SRC = files(
  'src/sp/opengl-1.1/gl_draw.c',
  'src/sp/opengl-1.1/gl_mesh.c',
  'src/sp/opengl-1.1/gl_model.c',
  'src/sp/opengl-1.1/gl_ngraph.c',
  'src/sp/opengl-1.1/gl_refrag.c',
  'src/sp/opengl-1.1/gl_rlight.c',
  'src/sp/opengl-1.1/gl_rmain.c',
  'src/sp/opengl-1.1/gl_rmisc.c',
  'src/sp/opengl-1.1/gl_rsurf.c',
  'src/sp/opengl-1.1/gl_screen.c',
  'src/sp/opengl-1.1/gl_warp.c',
  'src/sp/opengl-1.1/gl_vidlinuxglfw.c'
)

OGL_4_1_SRC = files(
  'src/sp/opengl-4.1/gl_draw.c',
  'src/sp/opengl-4.1/gl_mesh.c',
  'src/sp/opengl-4.1/gl_model.c',
  'src/sp/opengl-4.1/gl_ngraph.c',
  'src/sp/opengl-4.1/gl_refrag.c',
  'src/sp/opengl-4.1/gl_rlight.c',
  'src/sp/opengl-4.1/gl_rmain.c',
  'src/sp/opengl-4.1/gl_rmisc.c',
  'src/sp/opengl-4.1/gl_rsurf.c',
  'src/sp/opengl-4.1/gl_screen.c',
  'src/sp/opengl-4.1/gl_warp.c',
  'src/sp/opengl-4.1/gl_vidlinuxglfw.c'
)

if get_option('oglversion') == '1.1'
  OGL_SRC = OGL_1_1_SRC
elif get_option('oglversion') == '4.1'
  OGL_SRC = OGL_4_1_SRC
endif

UTIL_PAK_SRC = files(
  'src/util/pak/main.cpp',
  'src/util/pak/packfile.cpp'
)
#'src/sp/math.s'
  #  'src/sp/snd_mixa.c'

cc = meson.get_compiler('c')
m_dep = cc.find_library('m', required: false)

QWCL_DEP = [m_dep, dependency('GL'), dependency('glfw3'), dependency('glew')]

executable('qwcl', [QWCL_SRC, GLAD_SRC, OGL_SRC], include_directories: [QWCL_INC, QWCOMMON_INC, GLAD_INC], c_args: ['-DGLQUAKE', '-m64', '-g', '-ffast-math', '-funroll-loops', '-fomit-frame-pointer', '-fexpensive-optimizations', '-lm', '-lGL', '-lglew', '-lglfw'], dependencies: QWCL_DEP)
executable('qwsv', QWSV_SRC, include_directories: [QWSV_INC, QWCOMMON_INC, QWCL_INC], c_args: ['-DSERVERONLY', '-m64', '-g', '-ffast-math', '-funroll-loops', '-fomit-frame-pointer', '-fexpensive-optimizations'], dependencies: m_dep)

executable('pak', UTIL_PAK_SRC, include_directories: UTIL_PAK_INC)
