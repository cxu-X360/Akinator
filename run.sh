
pause()
{
	read -n 1 -s -r -p "${1:-Press any key to continue}"
	echo
}



clear_log()
{
	clear_dir logs/text_logs/
	clear_dir logs/dot-files/
	clear_dir logs/pictures/
	echo "logs deleted"
}

make_log_dirs()
{
    mkdir logs/dot-files
    mkdir logs/pictures
    mkdir logs/text_logs
}

make_log_dirs
clear
clear_log

tree_dir=includes/Tree
gcc -Wall -Wextra -Wshadow -Wconversion -Wunused "$1" $tree_dir/Tree_implement.c $tree_dir/debug_impliment.c $tree_dir/graph_manager.c $tree_dir/Tree_printer.c $tree_dir/Tree_reader.c -o main.out
pause
time ./main.out

./create_png.sh

#Это баш-скрипт - небольшая програмка для запуска и сборки всего проекта (пора бы заменять на мейк-файлы)