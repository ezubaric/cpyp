
# Train the language models
./dhpyplm_train german/verb_*.txt output.dlm 100
./incremental_query output.dlm german/test.txt output.tsv

