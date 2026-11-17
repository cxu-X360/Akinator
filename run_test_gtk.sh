pause()
{
	read -n 1 -s -r -p "${1:-Press any key to continue}"
	echo
}


clear
bash -c 'gcc -Wall -Wextra -Wshadow -Wconversion -Wunused $(pkg-config --cflags gtk4) working_onefile_test.c includes/Tree/Tree_implement.c includes/Tree/debug_impliment.c includes/Tree/graph_manager.c includes/Tree/Tree_printer.c includes/Tree/Tree_reader.c -o test_gtk.out $(pkg-config --libs gtk4)'
pause
./test_gtk.out 