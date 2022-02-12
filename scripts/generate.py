import json
import jinja2
import os
import argparse

def generate_module(path):
    print('generating template from {}'.format(path))
    with open(path) as file:
        module = json.load(file)

        env = jinja2.Environment(loader=jinja2.FileSystemLoader('template/'))
        
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
