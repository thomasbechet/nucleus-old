import json
import jinja2
import os
import argparse
import shutil

def generate_template(env, module, template, output_path):
    template = env.get_template(template)
    with open(output_path, 'w') as output:
        render = template.render(module=module)
        output.write(render)
        output.close()

def generate_module(path, install, install_implementation_source):
    print('Generating templates from {}'.format(path))
    with open(path) as file:
        # Load json
        module = json.load(file)

        # Resolve json
        define_interfaces = []
        for path in module['define_interfaces']:
            with open(os.path.normpath('{}/{}'.format('../', path))) as f:
                define_interfaces.append(json.load(f))
        module['define_interfaces'] = define_interfaces

        implement_interfaces = []
        for interface in module['implement_interfaces']:
            with open(os.path.normpath('{}/{}'.format('../', interface['path']))) as f:
                interface['definition'] = json.load(f)
                implement_interfaces.append(interface)
        module['implement_interfaces'] = implement_interfaces

        # Create jinja environment
        env = jinja2.Environment(loader=jinja2.FileSystemLoader('template/'))
        
        # Create module folder
        if not os.path.exists('module'):
            os.makedirs('module')

        # Generate files
        generate_template(env, module, 'module.h.template', 'module/module.h')
        generate_template(env, module, 'module.c.template', 'module/module.c')
        generate_template(env, module, 'definition.h.template', 'module/definition.h')
        generate_template(env, module, 'interface.h.template', 'module/interface.h')
        generate_template(env, module, 'interface.h.template', 'module/interface.h')
        generate_template(env, module, 'implementation.h.template', 'module/implementation.h')
        generate_template(env, module, 'implementation.c.template', 'module/implementation.c')

        # Install files
        if install:
            install_path = os.path.normpath('../{}'.format(module['directory']))
            source_path = './module'
            for file in ['definition.h', 'interface.h', 'module.c', 'module.h', 'implementation.h']:
                shutil.copy(
                    os.path.normpath('{}/{}'.format(source_path, file)), 
                    os.path.normpath('{}/{}'.format(install_path, file)))
            print('Files installed to {}'.format(install_path))

            # Install implementation source file
            if install_implementation_source:
                shutil.copy(
                    os.path.normpath('{}/{}'.format(source_path, 'implementation.c')), 
                    os.path.normpath('{}/{}'.format(install_path, 'implementation.c')))
                print('Implementation file installed to {}'.format(install_path))

if __name__ == '__main__':
    # Setup parser
    parser = argparse.ArgumentParser()
    parser.add_argument('--module', nargs=1, help='generate module files from a module configuration')
    parser.add_argument('--install', action='store_true')
    parser.add_argument('--install-implementation', action='store_true')
    args = parser.parse_args()

    # Generate module
    if args.module:
        generate_module(args.module[0], args.install, args.install_implementation)
