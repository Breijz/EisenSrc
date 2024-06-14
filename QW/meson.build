project('EisenSrc', 'c')

QWSV_INC = include_directories('include/mp')
QWCL_INC = include_directories('include/sp')
QWCOMMON_INC = include_directories('include/common')

QWSV_SRC = files(
  'src/mp/pr_cmds.c',
  'src/mp/pr_edict.c',
  'src/mp/pr_exec.c',
  'src/mp/sv_init.c',
  'src/mp/sv_main.c',
  'src/mp/sv_nchan.c',
  'src/mp/sv_ents.c',
  'src/mp/sv_send.c',
  'src/mp/sv_move.c',
  'src/mp/sv_phys.c',
  'src/mp/sv_user.c',
  'src/mp/sv_ccmds.c',
  'src/mp/world.c',
  'src/mp/sys_unix.c',
  'src/mp/sv_model.c',
  'src/sp/cmd.c',
  'src/sp/common.c',
  'src/sp/crc.c',
  'src/sp/cvar.c',
  'src/sp/mathlib.c',
  'src/sp/md4.c',
  'src/sp/zone.c',
  'src/sp/pmove.c',
  'src/sp/pmovetst.c',
  'src/sp/net_chan.c',
  'src/sp/net_udp.c'
)

QWCL_SRC = files(
  'src/sp/cl_demo.c',
  'src/sp/cl_ents.c',
  'src/sp/cl_input.c',
  'src/sp/cl_main.c',
  'src/sp/cl_parse.c',
  'src/sp/cl_pred.c',
  'src/sp/cl_tent.c',
  'src/sp/cl_cam.c',
  'src/sp/cmd.c',
  'src/sp/common.c',
  'src/sp/console.c',
  'src/sp/crc.c',
  'src/sp/cvar.c',
  'src/sp/keys.c',
  'src/sp/mathlib.c',
  'src/sp/md4.c',
  'src/sp/menu.c',
  'src/sp/net_chan.c',
  'src/sp/net_udp.c',
  'src/sp/nonintel.c',
  'src/sp/pmove.c',
  'src/sp/pmovetst.c',
  'src/sp/r_part.c',
  'src/sp/sbar.c',
  'src/sp/skin.c',
  'src/sp/snd_dma.c',
  'src/sp/snd_mem.c',
  'src/sp/snd_mix.c',
  'src/sp/view.c',
  'src/sp/wad.c',
  'src/sp/zone.c',
  'src/sp/cd_linux.c',
  'src/sp/sys_linux.c',
  'src/sp/snd_linux.c',
  'src/sp/gl_draw.c',
  'src/sp/gl_mesh.c',
  'src/sp/gl_model.c',
  'src/sp/gl_ngraph.c',
  'src/sp/gl_refrag.c',
  'src/sp/gl_rlight.c',
  'src/sp/gl_rmain.c',
  'src/sp/gl_rmisc.c',
  'src/sp/gl_rsurf.c',
  'src/sp/gl_screen.c',
  'src/sp/gl_warp.c',
  'src/sp/gl_vidlinuxglfw.c'
)
#'src/sp/math.s'
  #  'src/sp/snd_mixa.c'

cc = meson.get_compiler('c')
m_dep = cc.find_library('m', required: false)

QWCL_DEP = [m_dep, dependency('GL'), dependency('glfw3')]

executable('qwsv', QWSV_SRC, include_directories: [QWSV_INC, QWCOMMON_INC, QWCL_INC], c_args: ['-DSERVER_ONLY', '-m64', '-g', '-ffast-math', '-funroll-loops', '-fomit-frame-pointer', '-fexpensive-optimizations'], dependencies: m_dep)
executable('qwcl', QWCL_SRC, include_directories: [QWCL_INC, QWCOMMON_INC, QWSV_INC], c_args: ['-DGLQUAKE', '-m64', '-g', '-ffast-math', '-funroll-loops', '-fomit-frame-pointer', '-fexpensive-optimizations', '-lm', '-lGL', '-lglfw'], dependencies: QWCL_DEP)
