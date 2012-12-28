
from collections import defaultdict
from glob import glob
from string import strip
import sys

from nltk.tokenize.treebank import TreebankWordTokenizer

VERBS = set(["geben", "helfen", "sagen", "machen", "arbeiten", "bringen"])

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

    good_verbs = [x for x in sentences if len(sentences[x]) > 20]

    test_file = open("german/test.txt", 'w')

    print("COUNT:")
    for ii in good_verbs:
        print("%s\t%i" % (ii, len(sentences[ii])))

        o = open("german/verb_%s.txt" % ii, 'w')

        for (jj, sent) in enumerate(sentences[ii]):
            if jj % 5 == 0:
                test_file.write("%s\n" % ' '.join(sent))
            else:
                o.write("%s\n" % ' '.join(sent))
