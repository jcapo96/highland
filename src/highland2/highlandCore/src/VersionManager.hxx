#ifndef VERSIONMANAGER_HXX_SEEN
#define VERSIONMANAGER_HXX_SEEN
//#include "Versioning.hxx"
#include "ManagerBase.hxx"
#include "BasicTypes.hxx"

//-------------------------------------------------------------------

/// Maximum number of packages that can be added
const UInt_t NMAXPACKAGES=20;

typedef Int_t ProdId_h;

const ProdId_h UNKNOWN_PRODUCTION = -1;

class PackageVersion{
public:

  PackageVersion();

  // constructor provided name and version
  PackageVersion(const std::string& name, const std::string& version);
  
  /// Everyone should have a destructor.
  virtual ~PackageVersion() {}
  
  /// Return the name of this package. 
  const char* GetName() const {return _name.c_str();}

  /// Return the name of this package.
  const std::string& Name() const {return _name;}
  
  /// Set the name of this package.
  void SetName(const std::string& name) {_name = name;}
  
  /// Return the version of this package.
  const std::string& Version() const {return _version;}
  
  /// Set the version of this package.
  void SetVersion(const std::string& version) {_version = version;}
  
protected:
  
  /// The name of this package.
  std::string _name;

  /// The name of this package.
  std::string _version;
};


class Versioning: public ManagerBase{

public:

  virtual ~Versioning(){}

  /// return the singleton
  static Versioning& Get(void);

  /// Check compatibility between input file software version and the production version use in reader requirements file
  bool CheckVersionCompatibility(ProdId_h prodInput, ProdId_h prodReader);

  /// Add a production
  void AddProduction(ProdId_h prodId, const std::string& prodName, const std::string& nd280Version, const std::string& readerVersion);

  /// Get the enum of the production corresponding to a given sofware version
  ProdId_h GetProduction(const std::string& softVersion);

  /// Get the name of the production corresponding to a given sofware version
  std::string GetProductionName(const std::string& softVersion);

  /// Add a package
  void AddPackage(const std::string& name, const std::string& version);

  /// Get the version of a given package
  std::string GetPackageVersion(const std::string& name);
  
  /// Read package names and versions use to produce this file
  void ReadVersions(const std::string& file);

  /// dump package names and versions
  void DumpVersions();

 private:
  
  /// The singleton instance.
  static Versioning* _versioning;

  /// Private constructor as this is a singleton.
  Versioning();

  /// Unimplemented copy constructor as this is a singleton.
  Versioning(Versioning const&);

  /// Unimplemented assignment operator as this is a singleton.
  void operator=(Versioning const&);

  /// Get the software version range for a given production name
  bool GetSoftwareVersionsForProduction(ProdId_h prodId, std::string& nd280Version, std::string& readerVersion);

  /// Convert the production enum into an string
  std::string ConvertProduction(ProdId_h prod);

  /// Check wether a software version is prior to another
  bool CheckBeforeVersion(const std::string& version, const std::string& version0);

  /// Parse the version, revision and patch numbers
  void ParseSoftwareVersion(const std::string& version, int& v, int& r, int& p);

  /// vector of sofware version corresponding to each production
  std::vector<std::string> _prodnd280Version;
  std::vector<std::string> _prodReaderVersion;

  /// vector of production enums
  std::vector<ProdId_h> _prodId;

  /// vector of production names
  std::vector<std::string> _prodName;

  /// vector of versions of different packages
  std::vector<PackageVersion*> _packages;
};


namespace ND{
  Versioning& versioning();
}

#endif
