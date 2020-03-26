#ifndef ConfigTreeTools_h
#define ConfigTreeTools_h

#define VERSION_HAS_GETWEIGHTNAME // used in xsTool

#include <stdio.h>
#include <iostream>
#include "SystematicManager.hxx"
#include "ConfigurationManager.hxx"

class ConfigTreeTools{
 public :

  ConfigTreeTools(){_fromFile=false;}
  ConfigTreeTools(const std::string& fileName);
  ConfigTreeTools(SystematicManager& syst, ConfigurationManager& conf);

  // Some features need those managers
  void Initialize(const std::string& fileName);
  void Initialize(SystematicManager& syst, ConfigurationManager& conf);

  virtual ~ConfigTreeTools(){if (_fromFile){ delete _syst; delete _conf;}}

  /// Get the Index of a given weight for a given configuration in the micro-trees.
  /// To compute that it uses the config tree
  Int_t GetWeightIndex(const std::string& conf, const std::string& name) const;
  Int_t GetWeightIndex(Int_t conf, Int_t weight) const;

  /// Get the name of a weight provided the configuration and the number of this weight in that configuration
  std::string GetWeightName(const std::string& conf, Int_t weight_nbr) const;

  /// Get the number of weights in a given configuration
  Int_t GetNWeights(const std::string& conf) const;

  /// Get the value of a Float variable stored in the tree
  Float_t GetVarValueF(const std::string& name) const;

  /// Get the value of a Double variable stored in the tree
  Double_t GetVarValueD(const std::string& name) const;

  /// Get the value of a Int variable stored in the tree
  Int_t GetVarValueI(const std::string& name) const;

  /// Get the value of a Char variable stored in the tree
  std::string GetVarValueC(const std::string& name) const;
  
protected:

  SystematicManager* _syst;
  ConfigurationManager* _conf;

  std::string _fileName;

  bool _fromFile;
};



#endif


