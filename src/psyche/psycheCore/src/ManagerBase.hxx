#ifndef ManagerBase_h
#define ManagerBase_h

#include <TClonesArray.h>
#include <TChain.h>

class ManagerBase{
 public :

  ManagerBase(const std::string& tree_name, const std::string& vector_name, const std::string& class_name, void* ptr);
  virtual ~ManagerBase();


  virtual void ReadTreeVector(const std::string& file);
  virtual void WriteTreeVector(TTree& tree);  

  //  virtual void ReadTreeMap(const std::string& file);
  virtual void WriteTreeMap(TTree& tree);  
  
  
  /// Return the name of the tree
  const std::string& GetTreeName() const{return _treeName;}

    
protected:

  /// TChain used to read the "config" tree from the output file.
  TChain* _chain;

  /// Name of the Tree
  std::string _treeName;

  /// Name of the last file read
  std::string _fileName;

  std::string _className;

  std::string _vectorName;

  TBranch* _branch;

  void* _pointer;
  
};

#endif
