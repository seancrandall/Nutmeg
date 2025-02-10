#!/bin/bash

# Check if gentable exists in the current directory
if [ ! -f "./gentable" ]; then
    echo "Error: The script 'gentable' does not exist in the current directory."
    exit 1
fi

# Find all files ending with Model.h in the current directory
for result_file in *Model.h; do
    # Check if the file exists to avoid processing wildcard expansions that match nothing
    if [ -e "$result_file" ]; then
        # Remove "model.h" from the filename
        table_name=${result_file%model.h}
        
        # Call gentable with the modified filename
        ./gentable "$table_name"
        
        # Log the action
        echo "Regenerated $result_file"
    fi
done
