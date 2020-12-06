# If you create new headers/cpp files, add them to these list!
set(HEADER_FILES
	SrcMain.h
	fasta.h
	DNATrans.h
	alignment.h
)

set(SOURCE_FILES
	SrcMain.cpp
	fasta.cpp
	DNATrans.cpp
	alignment.cpp
)

# Don't change this
add_library(src ${SOURCE_FILES} ${HEADER_FILES})
