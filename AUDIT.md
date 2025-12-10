# Licence review methods

Best effort has been made to identify work originating from Aalto and non-Aalto affiliations. However, we want to note that mistakes are possible due to the necessity of human review of some of the files. To minimize the risk of accidental misattribution, we have chosen to remove the file from the copyright claim completely in any ambiguous cases.

Astaroth (git@bitbucket.org:jpekkila/astaroth.git) was forked from Astaroth Code (git@bitbucket.org:miikkavaisala/astaroth-code.git) in 2019.

## Astaroth Code

The repository git@bitbucket.org:miikkavaisala/astaroth-code.git was reviewed by listing all authors that had contributed to a file with

```bash
find . -type f ! -path '*/.*' | while read file; do echo -n "$file: "; git log --pretty=format:%an --full-history --no-merges -- $file | sort -u | xargs echo; done > file_ownership.txt
``` 

The contested files were listed with the following command.
```bash
rm -f contested_files.txt && \
find . -type f ! -path '*/.*' | while read file; do if git log --pretty=format:%an --full-history --no-merges -- $file | sort -u | is-contested ; then echo "$file" >> contested_files.txt; fi; done
```

Based on this information, we chose to remove the following directories
```bash
analysis \
config \
scripts \
doc \
samples \ # New directory for src/standalone
```

README.md and CMakeLists.txt in
```bash
./README.md \
./CMakeLists.txt
```

Furthermore, we removed all files matching the filenames listed with
```bash
rm -f contested_file_names.txt && \
find . -type f ! -path '*/.*' -not -name "README.md" -not -name "CMakeLists.txt" | while read file; do if git log --pretty=format:%an --full-history --no-merges -- $file | sort -u | is-contested ; then echo "$(basename $file)" >> contested_file_names.txt; fi; done
```


## Astaroth


#### Aalto Contributors
```
	"jpekkila",
    "Johannes Pekkilä",
    "Pekkilä Johannes",
    "Johannes Pekkila",
    "Matthias Rheinhardt",
    "mrheinhardt",
    "Maarit Korpi-Lagg",
    "Touko Puro",
    "Puro Touko",
    "purotouko",
    "ToxPuro",
    "toxpuro",
	"Frederick Gent",
    "fredagent",
	"fredgent",
    "Oskar Lappi", # until 2021-04-30
```

#### Removing clearly contested files
```bash
rm -r analysis \
test \
config \
bin \
acc-comm \
samples \
test-builds \
pilot \
scripts \
doc \
3rdparty \
deprecated \
cmake \
acc \
acc-runtime/samples/mhd_modular \
CMakeLists.txt \
README.md

# Removed files listed in contested_file_names (astaroth-code)
cat contested_file_names.txt | while read file; do 
    find . -type f ! -path '*/.*' -name "$file" -delete
done

find . -name "hashtable.h" -delete && \
find . -name "tinyexpr.*" -delete && \
find . -name "*kiss_fft*" -delete && find . -name "*kiss_fft*" -delete && \
find . -name "*.ac" -not -name "[0-9].ac" -not -name "blur.ac" -delete

# Remove further contested_files
rm ./include/astaroth_utils.h \
./src/core/node.cc \
./src/core/debug.cc \
./src/core/astaroth.cc \
./src/utils/modelsolver.cc \
./include/astaroth_debug.h

# Delete empty directories
find . -type d ! -path '*/.*' -empty -delete
```

#### Identifying further contested files

Files contributed to by Miikka Väisälä were identified with the following script
```bash
rm -f contested_files.txt && \
find . -type f ! -path '*/.*' | while read file; do if git log --pretty=format:%an --full-history --no-merges -- $file | sort -u | is-contested ; then echo "$file" >> contested_files.txt; fi; done
```
The results are in: contested_files_miikka.txt

How we addressed these files: contested_files_how_addressed.txt

Files contributed to by Oskar Lappi after 2021-04-30 were identified with the same command.
The results are in: contested_files_oskar_since_2021-04-31_how_addressed.txt
How we reviewed and addressed the files: contested_files_oskar_since_2021-04-31_how_addressed.txt


#### Removing contested lines of code

Then, we filtered the files through git blame and removed contested lines of code
```bash
find . -type f ! -path '*/.*' -not -name "tmp.txt" | while read file; do git blame --line-porcelain $file | process-blame > tmp.txt && mv tmp.txt $file; done
```

#### Removing the rest of possibly contested files

Finally, we went through the rest of files manually and, to our best efforts, removed files which could have contained contributions from non-Aalto authors.