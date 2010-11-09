// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * lemma.h - the definitions for lemma                          *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 ****************************************************************/

#ifndef _LEMMA_H
#define _LEMMA_H

#include "tokenizer.h"

/*===============================================================
 *
 * definitions about lemma 
 *
 *==============================================================*/

class CLemma {

private:
   /*===============================================================
    *
    * the tokenizer
    *
    *==============================================================*/
   
   class CStringTokenizer : public CTokenizer<string, 65537> {
      public: 
         CStringTokenizer() : CTokenizer<string, 65537>(1/*reserve for the other unknown */) {lookup("");} 
         virtual ~CStringTokenizer() {}
   };
   
protected:
   unsigned long m_nHash;

protected:
   // static method assigns tokenizer as global dictionary of words
   CStringTokenizer &getTokenizer() const {static CStringTokenizer tokenizer; return tokenizer;}

public:
   enum {UNKNOWN=0, EMPTY=1};

public:
   CLemma() { clear(); }
   CLemma(const string &s, bool bModify=true) : m_nHash(bModify ?  getTokenizer().lookup(s) : getTokenizer().find(s, UNKNOWN)) { }
   CLemma(const CLemma &w) : m_nHash(w.m_nHash) { }
   CLemma(const unsigned long &n) : m_nHash(n) { }
   virtual ~CLemma() {}

public:
   unsigned long hash() const { return m_nHash; }
   unsigned long code() const { return m_nHash; }
   bool operator == (const CLemma &w) const { return m_nHash == w.m_nHash; }
   bool operator != (const CLemma &w) const { return m_nHash != w.m_nHash; }
   bool operator < (const CLemma &w) const { return m_nHash < w.m_nHash; }
   bool operator = (const CLemma &w) { m_nHash =  w.m_nHash; }
   void copy(const CLemma &w) { m_nHash = w.m_nHash; }
   void setString(const string &s) { m_nHash = getTokenizer().find(s, UNKNOWN); }
   // do not use str() for unknown lemmas!!
   const string &str() const { ASSERT(m_nHash<getTokenizer().count(), "lemma.h: str() called for an unknown lemma"); return getTokenizer().key(m_nHash); }
   bool empty() { return m_nHash==EMPTY; }
   bool unknown() { return m_nHash==UNKNOWN; }
   void clear() { m_nHash=EMPTY; }
}; 

//===============================================================

inline istream & operator >> (istream &is, CLemma &w) {
   string s ;
   is >> s ;
   assert( s[0]=='[' && s[s.size()-1] == ']') ;
   w = s.substr(1,s.size()-2) ;
   return is ;
}

inline ostream & operator << (ostream &os, const CLemma &w) {
   os << '[' << w.str() << ']' ;
   return os ;
}

#endif
