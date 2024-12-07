project('bow', 'c',
  version : '0.1.0',
  default_options : [
    'c_std=c17'
  ]
)

# Embedded wraps
gtk4_wrap = '''
[wrap-git]
url = https://gitlab.gnome.org/GNOME/gtk.git
revision = 4.12.3
depth = 1
patch_directory = gtk4

[provide]
dependency_names = gtk4
'''

gtk4_layer_shell_wrap = '''
[wrap-git]
url = https://github.com/wmww/gtk4-layer-shell
revision = v1.0.1
depth = 1

[provide]
gtk_layer_shell = gtk_layer_shell_dep
'''

# Write wrap files programmatically
subprojects_dir = meson.current_source_dir() / 'subprojects'
run_command('mkdir', '-p', subprojects_dir, check: true)
run_command('tee', subprojects_dir / 'gtk4.wrap', input: gtk4_wrap, check: true)
run_command('tee', subprojects_dir / 'gtk4-layer-shell.wrap', input: gtk4_layer_shell_wrap, check: true)
  default_options: [
    'default_library=static',
    'media-gstreamer=disabled',
    'vulkan=disabled',
    'wayland-backend=true',
    'x11-backend=false',
    'broadway-backend=false',
    'build-demos=false',
    'build-examples=false',
    'build-tests=false'
  ]
)
gtk4_dep = gtk4_proj.get_variable('gtk_dep')

gtk4_layer_shell_proj = subproject('gtk4-layer-shell',
  default_options: [
    'default_library=static',
    'examples=false',
    'docs=false',
    'tests=false'
  ]
)
gtk4_layer_shell_dep = gtk4_layer_shell_proj.get_variable('gtk_layer_shell')

# Source files
src_files = [
  'src/main.c',
  'src/window.c',
  'src/log.c',
  'src/config.c',
  'src/message.c'
]

# Executable
executable('bow',
  src_files,
  dependencies: [
    gtk4_dep,
    gtk4_layer_shell_dep
  ],
  c_args: ['-g']
)

