/***************************************************************************
                          main.cpp  -  description
                             -------------------
    begin                : Сбт Дек 13 MSK 2003
    copyright            : (C) 2003 by Evgeney
    email                : dushistov@mail.ru
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "libcore.hpp"

#include <cstdlib>
#include <iostream>
#include <clocale>

#ifdef HAVE_NATSPEC
#include <natspec.h>
#endif

#include "utils.hpp"


class PrintUTF8{
  std::string utf8_str;
public:
  PrintUTF8(const std::string & s):utf8_str(s){}
  friend std::ostream &operator<<(std::ostream &os, PrintUTF8);
};


std::ostream &operator<<(std::ostream &os, PrintUTF8 print_utf8)
{
#ifndef HAVE_NATSPEC
  gsize bytes_read, bytes_written;
  GError *err=NULL;


  gchar *res=g_locale_from_utf8(print_utf8.utf8_str.c_str(), -1, &bytes_read, &bytes_written, &err);
  if(NULL==res){
    std::cerr<<"Can not convert "<<print_utf8.utf8_str<<" to current locale.\n"<<err->message<<std::endl;
    g_error_free(err);
    exit(EXIT_FAILURE);
  }
#else
  gchar *res = natspec_convert_with_translit(print_utf8.utf8_str.c_str(), NULL, "UTF-8");
#endif
  os << res;
  g_free(res); 
  return os;
}

void process_phrase(const char *loc_str, LibCore & lib)
{
  if(NULL==loc_str)
    return;
#ifndef HAVE_NATSPEC
  gsize bytes_read;
  gsize bytes_written;
  GError *err=NULL;
  char *str=g_locale_to_utf8(loc_str, -1, &bytes_read, &bytes_written, &err);
  if(NULL==str){
    std::cerr<<"Can not convert "<<loc_str<<" to utf8.\n"<<err->message<<std::endl;
    g_error_free(err);
    return;
  }
#else
  gchar *str = natspec_convert_with_translit(loc_str, "UTF-8", NULL);
#endif
  if(str[0]=='\0')
    return;
  std::vector<LibCore::SearchResult> res;
  bool is_found=false;
  if(str[0]=='/'){
    is_found=lib.LookupWithFuzzy(str+1, res);
  }
  else if(bContainRule(str)){
    is_found=lib.LookupWithRule(str, res);
  }
  else{
    if(!(is_found=lib.SimpleLookup(str, res)))
      is_found=lib.LookupWithFuzzy(str, res);
  }
  if(is_found){
    std::cout<<"Found "<<res.size()<<" items, similar to "<<PrintUTF8(str)<<"."<<std::endl;

    if(res.size()>1){
      for(size_t i=0; i<res.size(); ++i)
	std::cout<<PrintUTF8(res[i].phrase)<<"("<<i<<")"<<std::endl;
      int choise;
      for(;;){
	std::cout<<"Your choice: ";
	if(std::cin>>choise && choise>=0 && choise<int(res.size())){
	  std::cout<<PrintUTF8(res[choise].explanation)<<std::endl;
	  break;
	}
	else{
	  std::cin.clear();
	  char ch;
	  while(std::cin.get(ch) && ch!='\n');
	  std::cout<<"Invalid choise."<<std::endl<<"It must be from 0 to "<<res.size()-1<<"."<<std::endl;
	}
      }
    }
    else{
      std::cout<<PrintUTF8(res[0].explanation)<<std::endl;
    }
  }
  else{
    std::cout<<"Nothing similar to "<<PrintUTF8(str)<<", sorry :("<<std::endl;
  }
  g_free(str);
}

int main(int argc, char *argv[])
{
  setlocale(LC_ALL, "");
  LibCore lib;
  if(argc!=1 && argc!=2){
    std::cerr<<"Usage: "<<argv[0]<<" word"<<std::endl;
    return EXIT_FAILURE;
  }

  if(argc==2){
    process_phrase(argv[1], lib); 
  }
  else{
    std::string phrase;

    std::cout<<"Enter word or phrase: ";
    while(getline(std::cin, phrase)){
      process_phrase(phrase.c_str(), lib);
      std::cout<<"Enter word or phrase: ";
    }
  }

  return EXIT_SUCCESS;
}
