
from collections import defaultdict
from glob import glob
from string import strip
import sys

from nltk.tokenize.treebank import TreebankWordTokenizer

VERBS = set(["geben", "helfen", "sagen", "passieren", "schicken", "bringen"])

tokenizer = TreebankWordTokenizer()

if __name__ == "__main__":
    pattern = sys.argv[1]

    sentences = defaultdict(list)

    for ii in glob(pattern):
        for jj in map(strip, open(ii)):
            if any(jj.endswith("%s." % verb) for verb in VERBS):
                sentence = jj.split(".")[-2]
                words = tokenizer.tokenize(sentence)
                sentences[words[-1]].append(words)

    print("COUNT:")
    for ii in sentences:
        print("%s\t%i" % (ii, len(sentences[ii])))


