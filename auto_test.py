import subprocess
import time
import os
import threading
import shutil

def start_server(database_name):
    os.chdir('build')
    server_process = subprocess.Popen(
        ['./bin/rmdb', database_name],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True
    )
    time.sleep(1)
    return server_process

def start_client():
    os.chdir('../rmdb_client/build')
    client_process = subprocess.Popen(
        ['./rmdb_client'],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True
    )
    time.sleep(1)
    return client_process

def print_output(process, name, side):
    term_width = shutil.get_terminal_size().columns
    width = term_width // 2
    while True:
        output = process.stdout.readline()
        if output == '' and process.poll() is not None:
            break
        if output:
            if side == 'left':
                print(f"[{name}] {output.strip():<{width}}")
            else:
                print(f"{'':<{width}}[{name}] {output.strip()}")

def run_tests(client_process, commands):
    try:
        for command in commands:
            client_process.stdin.write(command + '\n')
            client_process.stdin.flush()
            
            time.sleep(0.1)
    except Exception as e:
        print(f"An error occurred: {e}")

def close_client(client_process):
    client_process.stdin.write('exit\n')
    client_process.stdin.flush()
    client_process.terminate()

def close_server(server_process):
    server_process.terminate()

if __name__ == "__main__":
    database_name = 'test_database'
    test_commands = [
		'create table concurrency_test (id int, name char(8), score float);',
        'insert into concurrency_test values (1, \'xiaohong\', 90.0);',
        'insert into concurrency_test values (2, \'xiaoming\', 95.0);',
        'insert into concurrency_test values (3, \'zhanghua\', 88.5);',

		# 'drop table concurrency_test;',
        # 更多测试命令
    ]
    
    server_process = start_server(database_name)
    server_thread = threading.Thread(target=print_output, args=(server_process, "Server", 'left'))
    server_thread.start()
    
    client_process = start_client()
    client_thread = threading.Thread(target=print_output, args=(client_process, "Client", 'right'))
    client_thread.start()
    
    run_tests(client_process, test_commands)
    
    close_client(client_process)
    close_server(server_process)
    
    client_thread.join()
    server_thread.join()
