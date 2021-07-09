import argparse
import re
import os
import sys

basename_pattern = re.compile(r'([^\.]+)(\.[^\.]+)?')
snake_case_pattern = re.compile(r'(?<!^)((?=[A-Z])|[\.\-_])')
illegal_identifier_char_sequences_pattern = re.compile(r'(^[0-9]+|[^a-zA-Z0-9_])')

def is_binary(path):
    try:
        with open(path, 'r') as file:
            file.read()
            return False
    except UnicodeDecodeError: 
        return True

def get_size_var_name(var_name):
    if re.match(r".*[A-Z]", var_name):
        return var_name + '_SIZE' 
    else:
        return var_name + 'Size'

def normalize_line_endings(text):
    return text.strip().replace('\r\n', '\n')

def escape_character(ch):
    switcher = {
        '\t': '\\t',
        '\r': '\\r',
        '\n': '\\n',
        '\'': '\\\'',
        '\\': '\\\\'
    }
    return switcher.get(ch, ch)

def convert_binary_resource(input_path, output_path, var_name, max_initializer_items_per_line):
    with open(input_path, 'rb') as reader:
        content = reader.read()

    with open(output_path, 'w') as writer:
        writer.write('#include <stdint.h>\n')
        writer.write('#include <stddef.h>\n\n')

        writer.write('uint8_t const ' + var_name + '[] = {')
        if len(content) == 0:
           writer.write(' 0 ')
        elif len(content) <= max_initializer_items_per_line:
            writer.write(' ')
        else:
            writer.write('\n\t')

        write_pos = 0
        for byte in content:
            writer.write("0x{:02x}".format(byte))

            write_pos += 1
            if write_pos < len(content):
                if write_pos % max_initializer_items_per_line > 0:
                    writer.write(', ')
                else:
                    writer.write(',\n\t')
            else:
                if len(content) > max_initializer_items_per_line:
                    writer.write('\n')
                else:
                    writer.write(' ')
                         
        writer.write('};\n\n')

        writer.write('size_t ' + var_name + '_SIZE = {};\n'.format(len(content)))

def convert_text_resource(input_path, output_path, var_name, max_initializer_items_per_line):
    with open(input_path, 'r') as reader:
        content = normalize_line_endings(reader.read())

    with open(output_path, 'w') as writer:
        writer.write('char const ' + var_name + '[] = {')
        if len(content) == 0:
            writer.write(' 0 ')
        elif len(content) + 1 <= max_initializer_items_per_line:
            writer.write(' ')
        else:
            writer.write('\n\t')

        write_pos = 0
        for char in content:
            writer.write('\'')
            writer.write(escape_character(char))
            writer.write('\'')
            
            write_pos += 1
            if write_pos % max_initializer_items_per_line > 0:
                writer.write(', ')
            else:
                writer.write(',\n\t')
        
        writer.write('\'\\0\'')
        write_pos +=1;
			
        if len(content) + 1 > max_initializer_items_per_line:
            writer.write('\n')
        else:
            writer.write(' ')

        writer.write('};\n')

def run(input_path, output_path, mode, var_name, max_initializer_items_per_line):
    if mode == 'b':
        convert_binary_resource(input_path, output_path, var_name, max_initializer_items_per_line)
    else:
        convert_text_resource(input_path, output_path, var_name, max_initializer_items_per_line)

def cli():
    parser = argparse.ArgumentParser(description='Convert a textual or binary resource into a C source file with a global C string or unsigned char array variable that yields the resource\'s content.')
    parser.add_argument('input_path', metavar='PATH', help='path of resource to be converted')
    parser.add_argument('-o', '--output', metavar='PATH', dest='output_path', help='path of resulting C source file (default: path of resource with *.c as extension)')
    parser.add_argument('-m', '--mode', metavar='MODE', choices=['b', 't'], help='\'b\': resource contains binary data, \'t\': resource contains text (default: automatically decected based on the resource\'s content type)')
    parser.add_argument('-v', '--variable', metavar='NAME', dest='var_name', help='name of global C string or unsigned char array variable in resulting C source file (default: base name of resource in all caps snake case with all non C\'ish characers removed)')
    parser.add_argument('--max_initializer_items_per_line', metavar='NUMBER', default=16, help='max. number of literal items per line in the initializer of global C string or unsigned char array variable (default: 16)')
    args = parser.parse_args()

    if args.output_path == None:
        args.output_path = basename_pattern.sub(r"\1.c", args.input_path)
    if args.mode == None:
        args.mode == 'b' if is_binary(args.input_path) else 't'
    if args.var_name == None:
        args.var_name = illegal_identifier_char_sequences_pattern.sub('', snake_case_pattern.sub('_', basename_pattern.sub(r"\1", args.input_path)).upper())

    if not os.path.exists(args.input_path):
        print(parser.prog + ': error: input file does not exist') 
        sys.exit(1)
    
    run(args.input_path, args.output_path, args.mode, args.var_name, args.max_initializer_items_per_line)

if __name__ == '__main__':
    cli()
