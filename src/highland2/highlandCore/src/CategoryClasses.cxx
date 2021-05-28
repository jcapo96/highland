#include "CategoryClasses.hxx"

//********************************************************************
std::string* TrackCategoryDefinition::GetNames(std::string names[]){
//********************************************************************

//  std::string* names = new std::string[20];
  for (UInt_t i=0;i<GetNTypes();i++){
    names[i] = _types[i]._name;
  }
  return names;
}


//********************************************************************
Int_t* TrackCategoryDefinition::GetCodes(Int_t codes[]){
//********************************************************************

//  Int_t* codes = new Int_t[20];
  for (UInt_t i=0;i<GetNTypes();i++){
    codes[i] = _types[i]._code;
  }
  return codes;
}


//********************************************************************
Int_t* TrackCategoryDefinition::GetColors(Int_t colors[]){
//********************************************************************

//  Int_t* colors = new Int_t[20];
  for (UInt_t i=0;i<GetNTypes();i++){
    colors[i] = _types[i]._color;
  }
  return colors;
}


//********************************************************************
void TrackCategoryDefinition::SetObjectCode(int code, int defaultcode,
					    Int_t indx2, Int_t indx3){
//********************************************************************

  bool ok = false;
  
  //check user code exist or set it to default if it dosnt
  for (std::vector<TrackTypeDefinition>::iterator it = _types.begin(); it != _types.end(); it++) {
    if (it->_code == code) {
      ok = true;
      break;
      }
  }
  if (!ok) {
    code = defaultcode;
  }
  
  //fill array of codes depending of the object hierarchy
  if(_objectOrder == 1){  
    _codesOrder1.push_back(code);
  }
  
  else if(_objectOrder == 2){
    if(indx2 < _counterSize2){
      _codesOrder1.push_back(code);
      if(indx2 == _counterSize2-1){
	_codesOrder2.push_back(_codesOrder1);
	_codesOrder1.clear();
      }
    }
    else std::cout << "WARNING: Tried to fill category " << _name << " outside matrix size. Indeces were: " << indx2 << std::endl;
  }

  else if(_objectOrder == 3){
    
    if(indx3 < _counterSize3){
      _codesOrder1.push_back(code);
      if(indx3 == _counterSize3-1){
	_codesOrder2.push_back(_codesOrder1);
	_codesOrder1.clear();
      }
    }
    else std::cout << "WARNING: Tried to fill category " << _name << " outside matrix size. Indeces were: " << indx2 << ", " << indx3 << std::endl;

    if(indx2 < _counterSize2){
      if(indx2 == _counterSize2-1 && indx3 == _counterSize3-1){
	_codesOrder3.push_back(_codesOrder2);
	_codesOrder2.clear();
      }
    }
    else std::cout << "WARNING: Tried to fill category " << _name << " outside matrix size. Indeces were: " << indx2 << ", " << indx3 << std::endl;
  }
}
