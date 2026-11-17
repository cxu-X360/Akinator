clear_dir()
{
    DIR="$1"
    echo $DIR
    for file in "$DIR"*; do
		if [ -f "$file" ]; then
            rm $file
		fi
	done
}

echo
clear_dir logs/pictures/
echo "Create png starts"
	DIR="logs/dot-files/"
	PNG_FILE_NAME="logs/pictures/graph_pict"
	num_files=0

	for file in $(ls "$DIR" -tr); do
		file="$DIR$file"
		if [ -f "$file" ]; then
			((num_files++))
			echo "Dot file: $file"
			dot -Tpng "$file" -o "$PNG_FILE_NAME"_"$num_files".png
			echo PNG file: "$PNG_FILE_NAME"_"$num_files".png
			echo
		fi
	done
echo "Create png ends"