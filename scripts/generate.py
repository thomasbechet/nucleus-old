import json
import jinja2
import os
import argparse

def generate_module(path):
    print('generating template from {}'.format(path))
    with open(path) as file:
        # Load json
        module = json.load(file)

        # Resolve json
        define_interfaces = []
        for path in module['define_interfaces']:
            with open('{}/{}'.format('../', path)) as f:
                define_interfaces.append(json.load(f))
        module['define_interfaces'] = define_interfaces

        implement_interfaces = []
        for interface in module['implement_interfaces']:
            with open('{}/{}'.format('../', interface['path'])) as f:
                interface['definition'] = json.load(f)
                implement_interfaces.append(interface)
        module['implement_interfaces'] = implement_interfaces

        # Create jinja environment
        env = jinja2.Environment(loader=jinja2.FileSystemLoader('template/'))
        
        # Create module folder
        if not os.path.exists('module'):
            os.makedirs('module')

        # Generate module.h
        template = env.get_template('module.h.template')
        with open('module/module.h', 'w') as output:
            render = template.render(module=module)
            output.write(render)
            output.close()

        # Generate module.c
        template = env.get_template('module.c.template')
        with open('module/module.c', 'w') as output:
            render = template.render(module=module)
            output.write(render)
            output.close()

        # Generate definition.h
        template = env.get_template('definition.h.template')
        with open('module/definition.h', 'w') as output:
            render = template.render(module=module)
            output.write(render)
            output.close()

        # Generate interface.h
        template = env.get_template('interface.h.template')
        with open('module/interface.h', 'w') as output:
            render = template.render(module=module)
            output.write(render)
            output.close()

        # Generate implementation.h
        template = env.get_template('implementation.h.template')
        with open('module/implementation.h', 'w') as output:
            render = template.render(module=module)
            output.write(render)
            output.close()
        
        # Generate implementation.c
        template = env.get_template('implementation.c.template')
        with open('module/implementation.c', 'w') as output:
            render = template.render(module=module)
            output.write(render)
            output.close()

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--module', nargs=1,
                        help='generate module files from a module configuration')
    args = parser.parse_args()

    if args.module:
        generate_module(args.module[0])
