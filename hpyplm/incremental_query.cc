
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <cstdlib>

#include "dhpyplm.h"
#include "corpus/corpus.h"

#include "cpyp/boost_serializers.h"
#include <boost/serialization/vector.hpp>
#include <boost/archive/binary_iarchive.hpp>

#define kORDER 3

using namespace std;
using namespace cpyp;
typedef const int cint;
typedef unsigned int uint;
typedef const unsigned int cuint;

Dict dict;

int writeReport(cint id, cuint vocab_size, cint sos, cint eos,
                const vector<uint>& sentence,
                const vector<DAPYPLM<kORDER>>& lms,
                fstream* outfile) { // NOLINT
  vector<unsigned> ctx(kORDER - 1, sos);
  vector<double> probs(lms.size(), 0.0);
  int cnt=0;

  for (unsigned ii=0; ii<=sentence.size(); ++ii) {
    unsigned word = (ii < sentence.size() ? sentence[ii] : eos);
    if (word >= vocab_size) {
      cerr << "**OOV ";
      continue;
    }

    // Go through each of the language models
    for (uint vv=0; vv < lms.size(); ++vv) {
      double lp = log(lms[vv].prob(word, ctx)) / log(2);
      cerr << "p(" << dict.Convert(word) << " |";
      for (unsigned j = ctx.size() + 1 - kORDER; j < ctx.size(); ++j)
        cerr << ' ' << dict.Convert(ctx[j]);
      cerr << ", " << vv;
      cerr << ") = " << lp << endl;
      ctx.push_back(word);
      probs[vv] -= lp;
      (*outfile) << id << "\t" << ii << "\t" << dict.Convert(word) <<
          vv << "\t" << probs[vv] << endl;
    }
    if (word < vocab_size) cnt++;
  }
  return cnt;
}

int main(int argc, char** argv) {
  if (argc != 4) {
    cerr << argv[0] << " <input.lm> <test.txt> <output.tsv>\n\nCompute perplexity of a " << kORDER << "-gram HPYP LM\n";
    return 1;
  }
  MT19937 eng;
  string lm_file = argv[1];
  string test_file = argv[2];
  string output = argv[3];

  PYPLM<kORDER> latent_lm;
  //vector<unsigned> ctx(kORDER - 1, kSOS);

  cerr << "Reading LM from " << lm_file << " ...\n";
  ifstream ifile(lm_file.c_str(), ios::in | ios::binary);
  if (!ifile.good()) {
    cerr << "Failed to open " << lm_file << " for reading\n";
    return 1;
  }
  boost::archive::binary_iarchive ia(ifile);
  Dict dict;
  ia & dict;
  ia & latent_lm;
  unsigned num_domains = 0;
  ia & num_domains;
  vector<DAPYPLM<kORDER>> dlm(num_domains, DAPYPLM<kORDER>(latent_lm));
  for (unsigned i = 0; i < num_domains; ++i)
    ia & dlm[i];
  const unsigned max_iv = dict.max();
  const unsigned kSOS = dict.Convert("<s>");
  const unsigned kEOS = dict.Convert("</s>");
  set<unsigned> tv;
  vector<vector<unsigned> > test;
  ReadFromFile(test_file, &dict, &test, &tv);

  fstream outfile((output + ".topics").c_str(), ios::out);
  int test_sent = 0;
  for (auto& s : test) {
    writeReport(test_sent++, max_iv, kSOS, kEOS, s, dlm, &outfile);
  }

}
