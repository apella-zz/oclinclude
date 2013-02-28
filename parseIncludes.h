#ifndef _PARSEINCLUDES_H_
#define _PARSEINCLUDES_H
#include <string>
#include <vector>
#include <algorithm> /* for for_each */
#include <numeric> /*for accumulate */
#include <fstream>
#include <iostream>

namespace spiers{ /* protection from unintended ADL*/

typedef std::vector<std::string> slist;

/*forward declaration of the function. */
static const char* fparseIncludes(const char *filename);
/*forward declaration. Does the same but takes
  a string of text instead of the name of a file */
static const char* parseIncludes(const char *text);

static std::string concatLines(const std::string &l, const std::string &r) {
  return l + std::string("\n") + r;
}

static std::string unlines (const slist &strings) {
  return std::accumulate(++strings.begin(), strings.end(), *strings.begin(), concatLines);
}


static slist lines(const std::string &in) {
  slist res;
  if (in.size() == 0) return res;
  size_t p1 = 0;
  std::string rest = in;
  do {
    size_t p2 = in.find_first_of('\n', p1+1);
    res.push_back(in.substr(p1, (p2 - p1)));
    p1 = p2;
  } while (p1 != std::string::npos);
  return res;
}

/* checks if the first string starts with the second one */
static bool beginsWith(const std::string &str, const std::string &comp) {
  return (0 == str.find(comp, 0));
}

static bool isInclude(const std::string &in) {
  return beginsWith(in, std::string("#include"));
}

/* returns the empty string if there is no valid filename in the line */
static std::string readFilename(const std::string &in) {
  size_t p = in.find_first_of("<\"");
  if (p == std::string::npos)
    return std::string("");
  else
    p++;

  size_t p2 = in.find_first_of(">\"", p);
  if (p2 == std::string::npos)
    return std::string("");

  return in.substr(p, p2-p);
}

static slist readLinesFromFile(const std::string &in) {
  std::ifstream input(in.c_str());
  slist strings;
  
  if (input.is_open()) {
    while (!input.eof()) {
      std::string s;
      std::getline(input, s);
      strings.push_back(s);
    }
  }
  return strings;
}

static void processLine(std::string &in) {
  if (isInclude(in))
    in = fparseIncludes(readFilename(in).c_str());
}

/* the function that should be called */
static const char *fparseIncludes(const char *filename) {
  slist l = readLinesFromFile(filename);
  std::for_each(l.begin(), l.end(), processLine);
  return unlines(l).c_str();
}
static const char *parseIncludes(const char *text) {
  slist l = lines(text);
  std::for_each(l.begin(), l.end(), processLine);
  return unlines(l).c_str();
}

/* for testing purposes only */
//#define PARSEINCLUDES__H_TESTFUNCS
#ifdef PARSEINCLUDES__H_TESTFUNCS
/* test functions */
static bool t_isInclude() {
  return (isInclude("#include <iostream>") &&
          !isInclude("random crap"));
}
static bool t_readFilename() {
  std::string t1 = readFilename("#include <iostream>");
  std::string t2 = readFilename("#include \"banana\"\n");

  bool res = (("iostream" == t1)
              && ("banana" == t2));
  if (!res )
    std::cout << t1 << "\n" << t2 <<"\n";

  return res;
}
static bool t_concatLines() {
  return ("foo\nbar" == concatLines("foo", "bar"));
}

static bool t_unlines() {
  slist l;
  l.push_back("a"); l.push_back("b"); l.push_back("c");
  std::string res = unlines(l);
  bool t = ("a\nb\nc" == res);
  if (!t)
    std::cout << res;
  return t;
}
static bool t_lines() {
  std::string s = "a\nb\nc";
  slist l;
  l.push_back("a"); l.push_back("b"); l.push_back("c");
  return (l != lines(s));  
}

  /*test a function and print its name if it failed */
#define testFunc(Func)                          \
  do {                                          \
    if (!(Func())) {                            \
      std::cout << #Func << "\n";}}             \
  while (0)
#endif /*PARSEINCLUDES__H_TESTFUNCS */
}
/*const char *fparseIncludes(const char *filename) {
  return parseIncludes_::fparseIncludes(filename); }
const char *parseIncludes(const char *text) {
  return parseIncludes_::parseIncludes(text); }
*/
#endif /* _PARSEINCLUDES_H_ */
