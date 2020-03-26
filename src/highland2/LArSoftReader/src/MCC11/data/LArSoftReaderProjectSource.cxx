namespace std {}
using namespace std;
#include "LArSoftReaderProjectHeaders.h"

#include "LArSoftReaderLinkDef.h"

#include "LArSoftReaderProjectDict.cxx"

struct DeleteObjectFunctor {
   template <typename T>
   void operator()(const T *ptr) const {
      delete ptr;
   }
   template <typename T, typename Q>
   void operator()(const std::pair<T,Q> &) const {
      // Do nothing
   }
   template <typename T, typename Q>
   void operator()(const std::pair<T,Q*> &ptr) const {
      delete ptr.second;
   }
   template <typename T, typename Q>
   void operator()(const std::pair<T*,Q> &ptr) const {
      delete ptr.first;
   }
   template <typename T, typename Q>
   void operator()(const std::pair<T*,Q*> &ptr) const {
      delete ptr.first;
      delete ptr.second;
   }
};

#ifndef art__EventAuxiliary_cxx
#define art__EventAuxiliary_cxx
art::EventAuxiliary::EventAuxiliary() {
}
art::EventAuxiliary::EventAuxiliary(const EventAuxiliary & rhs)
   : id_(const_cast<EventAuxiliary &>( rhs ).id_)
   , time_(const_cast<EventAuxiliary &>( rhs ).time_)
   , isRealData_(const_cast<EventAuxiliary &>( rhs ).isRealData_)
   , experimentType_(const_cast<EventAuxiliary &>( rhs ).experimentType_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
art::EventAuxiliary::~EventAuxiliary() {
}
#endif // art__EventAuxiliary_cxx

#ifndef art__EventID_cxx
#define art__EventID_cxx
art::EventID::EventID() {
}
art::EventID::EventID(const EventID & rhs)
   : subRun_(const_cast<EventID &>( rhs ).subRun_)
   , event_(const_cast<EventID &>( rhs ).event_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
art::EventID::~EventID() {
}
#endif // art__EventID_cxx

#ifndef art__SubRunID_cxx
#define art__SubRunID_cxx
art::SubRunID::SubRunID() {
}
art::SubRunID::SubRunID(const SubRunID & rhs)
   : run_(const_cast<SubRunID &>( rhs ).run_)
   , subRun_(const_cast<SubRunID &>( rhs ).subRun_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
art::SubRunID::~SubRunID() {
}
#endif // art__SubRunID_cxx

#ifndef art__RunID_cxx
#define art__RunID_cxx
art::RunID::RunID() {
}
art::RunID::RunID(const RunID & rhs)
   : run_(const_cast<RunID &>( rhs ).run_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
art::RunID::~RunID() {
}
#endif // art__RunID_cxx

#ifndef art__Timestamp_cxx
#define art__Timestamp_cxx
art::Timestamp::Timestamp() {
}
art::Timestamp::Timestamp(const Timestamp & rhs)
   : timeLow_(const_cast<Timestamp &>( rhs ).timeLow_)
   , timeHigh_(const_cast<Timestamp &>( rhs ).timeHigh_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
art::Timestamp::~Timestamp() {
}
#endif // art__Timestamp_cxx

#ifndef art__SubRunAuxiliary_cxx
#define art__SubRunAuxiliary_cxx
art::SubRunAuxiliary::SubRunAuxiliary() {
}
art::SubRunAuxiliary::SubRunAuxiliary(const SubRunAuxiliary & rhs)
   : processHistoryID_(const_cast<SubRunAuxiliary &>( rhs ).processHistoryID_)
   , rangeSetID_(const_cast<SubRunAuxiliary &>( rhs ).rangeSetID_)
   , id_(const_cast<SubRunAuxiliary &>( rhs ).id_)
   , beginTime_(const_cast<SubRunAuxiliary &>( rhs ).beginTime_)
   , endTime_(const_cast<SubRunAuxiliary &>( rhs ).endTime_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
art::SubRunAuxiliary::~SubRunAuxiliary() {
}
#endif // art__SubRunAuxiliary_cxx

#ifndef art__Hash_2__cxx
#define art__Hash_2__cxx
art::Hash<2>::Hash() {
}
art::Hash<2>::Hash(const Hash & rhs)
   : hash_(const_cast<Hash &>( rhs ).hash_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Hash &modrhs = const_cast<Hash &>( rhs );
   modrhs.hash_.clear();
}
art::Hash<2>::~Hash() {
}
#endif // art__Hash_2__cxx

#ifndef art__RunAuxiliary_cxx
#define art__RunAuxiliary_cxx
art::RunAuxiliary::RunAuxiliary() {
}
art::RunAuxiliary::RunAuxiliary(const RunAuxiliary & rhs)
   : processHistoryID_(const_cast<RunAuxiliary &>( rhs ).processHistoryID_)
   , allEventsProcessHistories_(const_cast<RunAuxiliary &>( rhs ).allEventsProcessHistories_)
   , rangeSetID_(const_cast<RunAuxiliary &>( rhs ).rangeSetID_)
   , id_(const_cast<RunAuxiliary &>( rhs ).id_)
   , beginTime_(const_cast<RunAuxiliary &>( rhs ).beginTime_)
   , endTime_(const_cast<RunAuxiliary &>( rhs ).endTime_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   RunAuxiliary &modrhs = const_cast<RunAuxiliary &>( rhs );
   modrhs.allEventsProcessHistories_.clear();
}
art::RunAuxiliary::~RunAuxiliary() {
}
#endif // art__RunAuxiliary_cxx

#ifndef art__ResultsAuxiliary_cxx
#define art__ResultsAuxiliary_cxx
art::ResultsAuxiliary::ResultsAuxiliary() {
}
art::ResultsAuxiliary::ResultsAuxiliary(const ResultsAuxiliary & rhs)
   : processHistoryID_(const_cast<ResultsAuxiliary &>( rhs ).processHistoryID_)
   , allEventsProcessHistories_(const_cast<ResultsAuxiliary &>( rhs ).allEventsProcessHistories_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   ResultsAuxiliary &modrhs = const_cast<ResultsAuxiliary &>( rhs );
   modrhs.allEventsProcessHistories_.clear();
}
art::ResultsAuxiliary::~ResultsAuxiliary() {
}
#endif // art__ResultsAuxiliary_cxx

#ifndef art__History_cxx
#define art__History_cxx
art::History::History() {
}
art::History::History(const History & rhs)
   : eventSelections_(const_cast<History &>( rhs ).eventSelections_)
   , branchListIndexes_(const_cast<History &>( rhs ).branchListIndexes_)
   , processHistoryID_(const_cast<History &>( rhs ).processHistoryID_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   History &modrhs = const_cast<History &>( rhs );
   modrhs.eventSelections_.clear();
   modrhs.branchListIndexes_.clear();
}
art::History::~History() {
}
#endif // art__History_cxx

#ifndef art__Wrapper_art__TriggerResults__cxx
#define art__Wrapper_art__TriggerResults__cxx
art::Wrapper<art::TriggerResults>::Wrapper() {
}
art::Wrapper<art::TriggerResults>::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
art::Wrapper<art::TriggerResults>::~Wrapper() {
}
#endif // art__Wrapper_art__TriggerResults__cxx

#ifndef art__EDProduct_cxx
#define art__EDProduct_cxx
art::EDProduct::EDProduct() {
}
art::EDProduct::EDProduct(const EDProduct & rhs)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
art::EDProduct::~EDProduct() {
}
#endif // art__EDProduct_cxx

#ifndef art__TriggerResults_cxx
#define art__TriggerResults_cxx
art::TriggerResults::TriggerResults() {
}
art::TriggerResults::TriggerResults(const TriggerResults & rhs)
   : art::HLTGlobalStatus(const_cast<TriggerResults &>( rhs ))
   , psetid_(const_cast<TriggerResults &>( rhs ).psetid_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
art::TriggerResults::~TriggerResults() {
}
#endif // art__TriggerResults_cxx

#ifndef art__HLTGlobalStatus_cxx
#define art__HLTGlobalStatus_cxx
art::HLTGlobalStatus::HLTGlobalStatus() {
}
art::HLTGlobalStatus::HLTGlobalStatus(const HLTGlobalStatus & rhs)
   : paths_(const_cast<HLTGlobalStatus &>( rhs ).paths_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   HLTGlobalStatus &modrhs = const_cast<HLTGlobalStatus &>( rhs );
   modrhs.paths_.clear();
}
art::HLTGlobalStatus::~HLTGlobalStatus() {
}
#endif // art__HLTGlobalStatus_cxx

#ifndef fhicl__ParameterSetID_cxx
#define fhicl__ParameterSetID_cxx
fhicl::ParameterSetID::ParameterSetID() {
}
fhicl::ParameterSetID::ParameterSetID(const ParameterSetID & rhs)
   : valid_(const_cast<ParameterSetID &>( rhs ).valid_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   for (Int_t i=0;i<20;i++) id_[i] = rhs.id_[i];
}
fhicl::ParameterSetID::~ParameterSetID() {
}
#endif // fhicl__ParameterSetID_cxx

#ifndef art__Wrapper_vector_artdaq__Fragment____cxx
#define art__Wrapper_vector_artdaq__Fragment____cxx
art::Wrapper<vector<artdaq::Fragment> >::Wrapper() {
}
art::Wrapper<vector<artdaq::Fragment> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Wrapper &modrhs = const_cast<Wrapper &>( rhs );
   modrhs.obj.clear();
}
art::Wrapper<vector<artdaq::Fragment> >::~Wrapper() {
}
#endif // art__Wrapper_vector_artdaq__Fragment____cxx

#ifndef artdaq__Fragment_cxx
#define artdaq__Fragment_cxx
artdaq::Fragment::Fragment() {
}
artdaq::Fragment::Fragment(const Fragment & rhs)
   : vals_(const_cast<Fragment &>( rhs ).vals_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
artdaq::Fragment::~Fragment() {
}
#endif // artdaq__Fragment_cxx

#ifndef artdaq__QuickVec_ULong64_t__cxx
#define artdaq__QuickVec_ULong64_t__cxx
artdaq::QuickVec<ULong64_t>::QuickVec() {
   data_ = 0;
}
artdaq::QuickVec<ULong64_t>::QuickVec(const QuickVec & rhs)
   : size_(const_cast<QuickVec &>( rhs ).size_)
   , data_(const_cast<QuickVec &>( rhs ).data_)
   , capacity_(const_cast<QuickVec &>( rhs ).capacity_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   QuickVec &modrhs = const_cast<QuickVec &>( rhs );
   modrhs.data_ = 0;
}
artdaq::QuickVec<ULong64_t>::~QuickVec() {
   delete [] data_;   data_ = 0;
}
#endif // artdaq__QuickVec_ULong64_t__cxx

#ifndef art__Wrapper_vector_artdaq__PackageBuildInfo____cxx
#define art__Wrapper_vector_artdaq__PackageBuildInfo____cxx
art::Wrapper<vector<artdaq::PackageBuildInfo> >::Wrapper() {
}
art::Wrapper<vector<artdaq::PackageBuildInfo> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Wrapper &modrhs = const_cast<Wrapper &>( rhs );
   modrhs.obj.clear();
}
art::Wrapper<vector<artdaq::PackageBuildInfo> >::~Wrapper() {
}
#endif // art__Wrapper_vector_artdaq__PackageBuildInfo____cxx

#ifndef artdaq__PackageBuildInfo_cxx
#define artdaq__PackageBuildInfo_cxx
artdaq::PackageBuildInfo::PackageBuildInfo() {
}
artdaq::PackageBuildInfo::PackageBuildInfo(const PackageBuildInfo & rhs)
   : packageName_(const_cast<PackageBuildInfo &>( rhs ).packageName_)
   , packageVersion_(const_cast<PackageBuildInfo &>( rhs ).packageVersion_)
   , buildTimestamp_(const_cast<PackageBuildInfo &>( rhs ).buildTimestamp_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   PackageBuildInfo &modrhs = const_cast<PackageBuildInfo &>( rhs );
   modrhs.packageName_.clear();
   modrhs.packageVersion_.clear();
   modrhs.buildTimestamp_.clear();
}
artdaq::PackageBuildInfo::~PackageBuildInfo() {
}
#endif // artdaq__PackageBuildInfo_cxx

#ifndef art__ProductProvenance_cxx
#define art__ProductProvenance_cxx
art::ProductProvenance::ProductProvenance() {
}
art::ProductProvenance::ProductProvenance(const ProductProvenance & rhs)
   : productID_(const_cast<ProductProvenance &>( rhs ).productID_)
   , productStatus_(const_cast<ProductProvenance &>( rhs ).productStatus_)
   , parentageID_(const_cast<ProductProvenance &>( rhs ).parentageID_)
   , transients_(const_cast<ProductProvenance &>( rhs ).transients_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
art::ProductProvenance::~ProductProvenance() {
}
#endif // art__ProductProvenance_cxx

#ifndef art__ProductID_cxx
#define art__ProductID_cxx
art::ProductID::ProductID() {
}
art::ProductID::ProductID(const ProductID & rhs)
   : value_(const_cast<ProductID &>( rhs ).value_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
art::ProductID::~ProductID() {
}
#endif // art__ProductID_cxx

#ifndef art__Hash_5__cxx
#define art__Hash_5__cxx
art::Hash<5>::Hash() {
}
art::Hash<5>::Hash(const Hash & rhs)
   : hash_(const_cast<Hash &>( rhs ).hash_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Hash &modrhs = const_cast<Hash &>( rhs );
   modrhs.hash_.clear();
}
art::Hash<5>::~Hash() {
}
#endif // art__Hash_5__cxx

#ifndef art__HLTPathStatus_cxx
#define art__HLTPathStatus_cxx
art::HLTPathStatus::HLTPathStatus() {
}
art::HLTPathStatus::HLTPathStatus(const HLTPathStatus & rhs)
   : status_(const_cast<HLTPathStatus &>( rhs ).status_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
art::HLTPathStatus::~HLTPathStatus() {
}
#endif // art__HLTPathStatus_cxx

#ifndef art__FileFormatVersion_cxx
#define art__FileFormatVersion_cxx
art::FileFormatVersion::FileFormatVersion() {
}
art::FileFormatVersion::FileFormatVersion(const FileFormatVersion & rhs)
   : value_(const_cast<FileFormatVersion &>( rhs ).value_)
   , era_(const_cast<FileFormatVersion &>( rhs ).era_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   FileFormatVersion &modrhs = const_cast<FileFormatVersion &>( rhs );
   modrhs.era_.clear();
}
art::FileFormatVersion::~FileFormatVersion() {
}
#endif // art__FileFormatVersion_cxx

#ifndef art__FileIndex__Element_cxx
#define art__FileIndex__Element_cxx
art::FileIndex::Element::Element() {
}
art::FileIndex::Element::Element(const Element & rhs)
   : eventID_(const_cast<Element &>( rhs ).eventID_)
   , entry_(const_cast<Element &>( rhs ).entry_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
art::FileIndex::Element::~Element() {
}
#endif // art__FileIndex__Element_cxx

#ifndef art__ProcessHistory_cxx
#define art__ProcessHistory_cxx
art::ProcessHistory::ProcessHistory() {
}
art::ProcessHistory::ProcessHistory(const ProcessHistory & rhs)
   : data_(const_cast<ProcessHistory &>( rhs ).data_)
   , transients_(const_cast<ProcessHistory &>( rhs ).transients_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   ProcessHistory &modrhs = const_cast<ProcessHistory &>( rhs );
   modrhs.data_.clear();
}
art::ProcessHistory::~ProcessHistory() {
}
#endif // art__ProcessHistory_cxx

#ifndef art__ProcessConfiguration_cxx
#define art__ProcessConfiguration_cxx
art::ProcessConfiguration::ProcessConfiguration() {
}
art::ProcessConfiguration::ProcessConfiguration(const ProcessConfiguration & rhs)
   : processName_(const_cast<ProcessConfiguration &>( rhs ).processName_)
   , parameterSetID_(const_cast<ProcessConfiguration &>( rhs ).parameterSetID_)
   , releaseVersion_(const_cast<ProcessConfiguration &>( rhs ).releaseVersion_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   ProcessConfiguration &modrhs = const_cast<ProcessConfiguration &>( rhs );
   modrhs.processName_.clear();
   modrhs.releaseVersion_.clear();
}
art::ProcessConfiguration::~ProcessConfiguration() {
}
#endif // art__ProcessConfiguration_cxx

#ifndef art__ProductRegistry_cxx
#define art__ProductRegistry_cxx
art::ProductRegistry::ProductRegistry() {
}
art::ProductRegistry::ProductRegistry(const ProductRegistry & rhs)
   : productList_(const_cast<ProductRegistry &>( rhs ).productList_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   ProductRegistry &modrhs = const_cast<ProductRegistry &>( rhs );
   modrhs.productList_.clear();
}
art::ProductRegistry::~ProductRegistry() {
}
#endif // art__ProductRegistry_cxx

#ifndef art__BranchKey_cxx
#define art__BranchKey_cxx
art::BranchKey::BranchKey() {
}
art::BranchKey::BranchKey(const BranchKey & rhs)
   : friendlyClassName_(const_cast<BranchKey &>( rhs ).friendlyClassName_)
   , moduleLabel_(const_cast<BranchKey &>( rhs ).moduleLabel_)
   , productInstanceName_(const_cast<BranchKey &>( rhs ).productInstanceName_)
   , processName_(const_cast<BranchKey &>( rhs ).processName_)
   , branchType_(const_cast<BranchKey &>( rhs ).branchType_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   BranchKey &modrhs = const_cast<BranchKey &>( rhs );
   modrhs.friendlyClassName_.clear();
   modrhs.moduleLabel_.clear();
   modrhs.productInstanceName_.clear();
   modrhs.processName_.clear();
}
art::BranchKey::~BranchKey() {
}
#endif // art__BranchKey_cxx

#ifndef art__BranchDescription_cxx
#define art__BranchDescription_cxx
art::BranchDescription::BranchDescription() {
}
art::BranchDescription::BranchDescription(const BranchDescription & rhs)
   : branchType_(const_cast<BranchDescription &>( rhs ).branchType_)
   , moduleLabel_(const_cast<BranchDescription &>( rhs ).moduleLabel_)
   , processName_(const_cast<BranchDescription &>( rhs ).processName_)
   , productID_(const_cast<BranchDescription &>( rhs ).productID_)
   , producedClassName_(const_cast<BranchDescription &>( rhs ).producedClassName_)
   , friendlyClassName_(const_cast<BranchDescription &>( rhs ).friendlyClassName_)
   , productInstanceName_(const_cast<BranchDescription &>( rhs ).productInstanceName_)
   , supportsView_(const_cast<BranchDescription &>( rhs ).supportsView_)
   , psetIDs_(const_cast<BranchDescription &>( rhs ).psetIDs_)
   , processConfigurationIDs_(const_cast<BranchDescription &>( rhs ).processConfigurationIDs_)
   , transients_(const_cast<BranchDescription &>( rhs ).transients_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   BranchDescription &modrhs = const_cast<BranchDescription &>( rhs );
   modrhs.moduleLabel_.clear();
   modrhs.processName_.clear();
   modrhs.producedClassName_.clear();
   modrhs.friendlyClassName_.clear();
   modrhs.productInstanceName_.clear();
   modrhs.psetIDs_.clear();
   modrhs.processConfigurationIDs_.clear();
}
art::BranchDescription::~BranchDescription() {
}
#endif // art__BranchDescription_cxx

#ifndef art__Hash_3__cxx
#define art__Hash_3__cxx
art::Hash<3>::Hash() {
}
art::Hash<3>::Hash(const Hash & rhs)
   : hash_(const_cast<Hash &>( rhs ).hash_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Hash &modrhs = const_cast<Hash &>( rhs );
   modrhs.hash_.clear();
}
art::Hash<3>::~Hash() {
}
#endif // art__Hash_3__cxx

#ifndef art__Parentage_cxx
#define art__Parentage_cxx
art::Parentage::Parentage() {
}
art::Parentage::Parentage(const Parentage & rhs)
   : parents_(const_cast<Parentage &>( rhs ).parents_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Parentage &modrhs = const_cast<Parentage &>( rhs );
   modrhs.parents_.clear();
}
art::Parentage::~Parentage() {
}
#endif // art__Parentage_cxx

#ifndef art__BranchChildren_cxx
#define art__BranchChildren_cxx
art::BranchChildren::BranchChildren() {
}
art::BranchChildren::BranchChildren(const BranchChildren & rhs)
   : childLookup_(const_cast<BranchChildren &>( rhs ).childLookup_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   BranchChildren &modrhs = const_cast<BranchChildren &>( rhs );
   modrhs.childLookup_.clear();
}
art::BranchChildren::~BranchChildren() {
}
#endif // art__BranchChildren_cxx

#ifndef art__Wrapper_art__Assns_raw__RawDigit_raw__RDTimeStamp_void____cxx
#define art__Wrapper_art__Assns_raw__RawDigit_raw__RDTimeStamp_void____cxx
art::Wrapper<art::Assns<raw::RawDigit,raw::RDTimeStamp,void> >::Wrapper() {
}
art::Wrapper<art::Assns<raw::RawDigit,raw::RDTimeStamp,void> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
art::Wrapper<art::Assns<raw::RawDigit,raw::RDTimeStamp,void> >::~Wrapper() {
}
#endif // art__Wrapper_art__Assns_raw__RawDigit_raw__RDTimeStamp_void____cxx

#ifndef art__Assns_raw__RawDigit_raw__RDTimeStamp_void__cxx
#define art__Assns_raw__RawDigit_raw__RDTimeStamp_void__cxx
art::Assns<raw::RawDigit,raw::RDTimeStamp,void>::Assns() {
}
art::Assns<raw::RawDigit,raw::RDTimeStamp,void>::Assns(const Assns & rhs)
   : art::detail::AssnsBase(const_cast<Assns &>( rhs ))
   , ptr_data_1_(const_cast<Assns &>( rhs ).ptr_data_1_)
   , ptr_data_2_(const_cast<Assns &>( rhs ).ptr_data_2_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Assns &modrhs = const_cast<Assns &>( rhs );
   modrhs.ptr_data_1_.clear();
   modrhs.ptr_data_2_.clear();
}
art::Assns<raw::RawDigit,raw::RDTimeStamp,void>::~Assns() {
}
#endif // art__Assns_raw__RawDigit_raw__RDTimeStamp_void__cxx

#ifndef art__detail__AssnsBase_cxx
#define art__detail__AssnsBase_cxx
art::detail::AssnsBase::AssnsBase() {
}
art::detail::AssnsBase::AssnsBase(const AssnsBase & rhs)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
art::detail::AssnsBase::~AssnsBase() {
}
#endif // art__detail__AssnsBase_cxx

#ifndef art__Wrapper_art__Assns_raw__RawDigit_recob__Hit_void____cxx
#define art__Wrapper_art__Assns_raw__RawDigit_recob__Hit_void____cxx
art::Wrapper<art::Assns<raw::RawDigit,recob::Hit,void> >::Wrapper() {
}
art::Wrapper<art::Assns<raw::RawDigit,recob::Hit,void> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
art::Wrapper<art::Assns<raw::RawDigit,recob::Hit,void> >::~Wrapper() {
}
#endif // art__Wrapper_art__Assns_raw__RawDigit_recob__Hit_void____cxx

#ifndef art__Assns_raw__RawDigit_recob__Hit_void__cxx
#define art__Assns_raw__RawDigit_recob__Hit_void__cxx
art::Assns<raw::RawDigit,recob::Hit,void>::Assns() {
}
art::Assns<raw::RawDigit,recob::Hit,void>::Assns(const Assns & rhs)
   : art::detail::AssnsBase(const_cast<Assns &>( rhs ))
   , ptr_data_1_(const_cast<Assns &>( rhs ).ptr_data_1_)
   , ptr_data_2_(const_cast<Assns &>( rhs ).ptr_data_2_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Assns &modrhs = const_cast<Assns &>( rhs );
   modrhs.ptr_data_1_.clear();
   modrhs.ptr_data_2_.clear();
}
art::Assns<raw::RawDigit,recob::Hit,void>::~Assns() {
}
#endif // art__Assns_raw__RawDigit_recob__Hit_void__cxx

#ifndef art__Wrapper_art__Assns_raw__RawDigit_recob__Wire_void____cxx
#define art__Wrapper_art__Assns_raw__RawDigit_recob__Wire_void____cxx
art::Wrapper<art::Assns<raw::RawDigit,recob::Wire,void> >::Wrapper() {
}
art::Wrapper<art::Assns<raw::RawDigit,recob::Wire,void> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
art::Wrapper<art::Assns<raw::RawDigit,recob::Wire,void> >::~Wrapper() {
}
#endif // art__Wrapper_art__Assns_raw__RawDigit_recob__Wire_void____cxx

#ifndef art__Assns_raw__RawDigit_recob__Wire_void__cxx
#define art__Assns_raw__RawDigit_recob__Wire_void__cxx
art::Assns<raw::RawDigit,recob::Wire,void>::Assns() {
}
art::Assns<raw::RawDigit,recob::Wire,void>::Assns(const Assns & rhs)
   : art::detail::AssnsBase(const_cast<Assns &>( rhs ))
   , ptr_data_1_(const_cast<Assns &>( rhs ).ptr_data_1_)
   , ptr_data_2_(const_cast<Assns &>( rhs ).ptr_data_2_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Assns &modrhs = const_cast<Assns &>( rhs );
   modrhs.ptr_data_1_.clear();
   modrhs.ptr_data_2_.clear();
}
art::Assns<raw::RawDigit,recob::Wire,void>::~Assns() {
}
#endif // art__Assns_raw__RawDigit_recob__Wire_void__cxx

#ifndef art__Wrapper_art__Assns_recob__Cluster_recob__EndPoint2D_unsigned_short____cxx
#define art__Wrapper_art__Assns_recob__Cluster_recob__EndPoint2D_unsigned_short____cxx
art::Wrapper<art::Assns<recob::Cluster,recob::EndPoint2D,unsigned short> >::Wrapper() {
}
art::Wrapper<art::Assns<recob::Cluster,recob::EndPoint2D,unsigned short> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
art::Wrapper<art::Assns<recob::Cluster,recob::EndPoint2D,unsigned short> >::~Wrapper() {
}
#endif // art__Wrapper_art__Assns_recob__Cluster_recob__EndPoint2D_unsigned_short____cxx

#ifndef art__Assns_recob__Cluster_recob__EndPoint2D_unsigned_short__cxx
#define art__Assns_recob__Cluster_recob__EndPoint2D_unsigned_short__cxx
art::Assns<recob::Cluster,recob::EndPoint2D,unsigned short>::Assns() {
}
art::Assns<recob::Cluster,recob::EndPoint2D,unsigned short>::Assns(const Assns & rhs)
   : art::Assns<recob::Cluster,recob::EndPoint2D,void>(const_cast<Assns &>( rhs ))
   , data_(const_cast<Assns &>( rhs ).data_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Assns &modrhs = const_cast<Assns &>( rhs );
   modrhs.data_.clear();
}
art::Assns<recob::Cluster,recob::EndPoint2D,unsigned short>::~Assns() {
}
#endif // art__Assns_recob__Cluster_recob__EndPoint2D_unsigned_short__cxx

#ifndef art__Assns_recob__Cluster_recob__EndPoint2D_void__cxx
#define art__Assns_recob__Cluster_recob__EndPoint2D_void__cxx
art::Assns<recob::Cluster,recob::EndPoint2D,void>::Assns() {
}
art::Assns<recob::Cluster,recob::EndPoint2D,void>::Assns(const Assns & rhs)
   : art::detail::AssnsBase(const_cast<Assns &>( rhs ))
   , ptr_data_1_(const_cast<Assns &>( rhs ).ptr_data_1_)
   , ptr_data_2_(const_cast<Assns &>( rhs ).ptr_data_2_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Assns &modrhs = const_cast<Assns &>( rhs );
   modrhs.ptr_data_1_.clear();
   modrhs.ptr_data_2_.clear();
}
art::Assns<recob::Cluster,recob::EndPoint2D,void>::~Assns() {
}
#endif // art__Assns_recob__Cluster_recob__EndPoint2D_void__cxx

#ifndef art__Wrapper_art__Assns_recob__Cluster_recob__Hit_void____cxx
#define art__Wrapper_art__Assns_recob__Cluster_recob__Hit_void____cxx
art::Wrapper<art::Assns<recob::Cluster,recob::Hit,void> >::Wrapper() {
}
art::Wrapper<art::Assns<recob::Cluster,recob::Hit,void> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
art::Wrapper<art::Assns<recob::Cluster,recob::Hit,void> >::~Wrapper() {
}
#endif // art__Wrapper_art__Assns_recob__Cluster_recob__Hit_void____cxx

#ifndef art__Assns_recob__Cluster_recob__Hit_void__cxx
#define art__Assns_recob__Cluster_recob__Hit_void__cxx
art::Assns<recob::Cluster,recob::Hit,void>::Assns() {
}
art::Assns<recob::Cluster,recob::Hit,void>::Assns(const Assns & rhs)
   : art::detail::AssnsBase(const_cast<Assns &>( rhs ))
   , ptr_data_1_(const_cast<Assns &>( rhs ).ptr_data_1_)
   , ptr_data_2_(const_cast<Assns &>( rhs ).ptr_data_2_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Assns &modrhs = const_cast<Assns &>( rhs );
   modrhs.ptr_data_1_.clear();
   modrhs.ptr_data_2_.clear();
}
art::Assns<recob::Cluster,recob::Hit,void>::~Assns() {
}
#endif // art__Assns_recob__Cluster_recob__Hit_void__cxx

#ifndef art__Wrapper_art__Assns_recob__Cluster_recob__Vertex_unsigned_short____cxx
#define art__Wrapper_art__Assns_recob__Cluster_recob__Vertex_unsigned_short____cxx
art::Wrapper<art::Assns<recob::Cluster,recob::Vertex,unsigned short> >::Wrapper() {
}
art::Wrapper<art::Assns<recob::Cluster,recob::Vertex,unsigned short> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
art::Wrapper<art::Assns<recob::Cluster,recob::Vertex,unsigned short> >::~Wrapper() {
}
#endif // art__Wrapper_art__Assns_recob__Cluster_recob__Vertex_unsigned_short____cxx

#ifndef art__Assns_recob__Cluster_recob__Vertex_unsigned_short__cxx
#define art__Assns_recob__Cluster_recob__Vertex_unsigned_short__cxx
art::Assns<recob::Cluster,recob::Vertex,unsigned short>::Assns() {
}
art::Assns<recob::Cluster,recob::Vertex,unsigned short>::Assns(const Assns & rhs)
   : art::Assns<recob::Cluster,recob::Vertex,void>(const_cast<Assns &>( rhs ))
   , data_(const_cast<Assns &>( rhs ).data_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Assns &modrhs = const_cast<Assns &>( rhs );
   modrhs.data_.clear();
}
art::Assns<recob::Cluster,recob::Vertex,unsigned short>::~Assns() {
}
#endif // art__Assns_recob__Cluster_recob__Vertex_unsigned_short__cxx

#ifndef art__Assns_recob__Cluster_recob__Vertex_void__cxx
#define art__Assns_recob__Cluster_recob__Vertex_void__cxx
art::Assns<recob::Cluster,recob::Vertex,void>::Assns() {
}
art::Assns<recob::Cluster,recob::Vertex,void>::Assns(const Assns & rhs)
   : art::detail::AssnsBase(const_cast<Assns &>( rhs ))
   , ptr_data_1_(const_cast<Assns &>( rhs ).ptr_data_1_)
   , ptr_data_2_(const_cast<Assns &>( rhs ).ptr_data_2_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Assns &modrhs = const_cast<Assns &>( rhs );
   modrhs.ptr_data_1_.clear();
   modrhs.ptr_data_2_.clear();
}
art::Assns<recob::Cluster,recob::Vertex,void>::~Assns() {
}
#endif // art__Assns_recob__Cluster_recob__Vertex_void__cxx

#ifndef art__Wrapper_art__Assns_recob__Hit_recob__SpacePoint_void____cxx
#define art__Wrapper_art__Assns_recob__Hit_recob__SpacePoint_void____cxx
art::Wrapper<art::Assns<recob::Hit,recob::SpacePoint,void> >::Wrapper() {
}
art::Wrapper<art::Assns<recob::Hit,recob::SpacePoint,void> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
art::Wrapper<art::Assns<recob::Hit,recob::SpacePoint,void> >::~Wrapper() {
}
#endif // art__Wrapper_art__Assns_recob__Hit_recob__SpacePoint_void____cxx

#ifndef art__Assns_recob__Hit_recob__SpacePoint_void__cxx
#define art__Assns_recob__Hit_recob__SpacePoint_void__cxx
art::Assns<recob::Hit,recob::SpacePoint,void>::Assns() {
}
art::Assns<recob::Hit,recob::SpacePoint,void>::Assns(const Assns & rhs)
   : art::detail::AssnsBase(const_cast<Assns &>( rhs ))
   , ptr_data_1_(const_cast<Assns &>( rhs ).ptr_data_1_)
   , ptr_data_2_(const_cast<Assns &>( rhs ).ptr_data_2_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Assns &modrhs = const_cast<Assns &>( rhs );
   modrhs.ptr_data_1_.clear();
   modrhs.ptr_data_2_.clear();
}
art::Assns<recob::Hit,recob::SpacePoint,void>::~Assns() {
}
#endif // art__Assns_recob__Hit_recob__SpacePoint_void__cxx

#ifndef art__Wrapper_art__Assns_recob__OpFlash_recob__OpHit_void____cxx
#define art__Wrapper_art__Assns_recob__OpFlash_recob__OpHit_void____cxx
art::Wrapper<art::Assns<recob::OpFlash,recob::OpHit,void> >::Wrapper() {
}
art::Wrapper<art::Assns<recob::OpFlash,recob::OpHit,void> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
art::Wrapper<art::Assns<recob::OpFlash,recob::OpHit,void> >::~Wrapper() {
}
#endif // art__Wrapper_art__Assns_recob__OpFlash_recob__OpHit_void____cxx

#ifndef art__Assns_recob__OpFlash_recob__OpHit_void__cxx
#define art__Assns_recob__OpFlash_recob__OpHit_void__cxx
art::Assns<recob::OpFlash,recob::OpHit,void>::Assns() {
}
art::Assns<recob::OpFlash,recob::OpHit,void>::Assns(const Assns & rhs)
   : art::detail::AssnsBase(const_cast<Assns &>( rhs ))
   , ptr_data_1_(const_cast<Assns &>( rhs ).ptr_data_1_)
   , ptr_data_2_(const_cast<Assns &>( rhs ).ptr_data_2_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Assns &modrhs = const_cast<Assns &>( rhs );
   modrhs.ptr_data_1_.clear();
   modrhs.ptr_data_2_.clear();
}
art::Assns<recob::OpFlash,recob::OpHit,void>::~Assns() {
}
#endif // art__Assns_recob__OpFlash_recob__OpHit_void__cxx

#ifndef art__Wrapper_art__Assns_recob__PFParticle_anab__T0_void____cxx
#define art__Wrapper_art__Assns_recob__PFParticle_anab__T0_void____cxx
art::Wrapper<art::Assns<recob::PFParticle,anab::T0,void> >::Wrapper() {
}
art::Wrapper<art::Assns<recob::PFParticle,anab::T0,void> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
art::Wrapper<art::Assns<recob::PFParticle,anab::T0,void> >::~Wrapper() {
}
#endif // art__Wrapper_art__Assns_recob__PFParticle_anab__T0_void____cxx

#ifndef art__Assns_recob__PFParticle_anab__T0_void__cxx
#define art__Assns_recob__PFParticle_anab__T0_void__cxx
art::Assns<recob::PFParticle,anab::T0,void>::Assns() {
}
art::Assns<recob::PFParticle,anab::T0,void>::Assns(const Assns & rhs)
   : art::detail::AssnsBase(const_cast<Assns &>( rhs ))
   , ptr_data_1_(const_cast<Assns &>( rhs ).ptr_data_1_)
   , ptr_data_2_(const_cast<Assns &>( rhs ).ptr_data_2_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Assns &modrhs = const_cast<Assns &>( rhs );
   modrhs.ptr_data_1_.clear();
   modrhs.ptr_data_2_.clear();
}
art::Assns<recob::PFParticle,anab::T0,void>::~Assns() {
}
#endif // art__Assns_recob__PFParticle_anab__T0_void__cxx

#ifndef art__Wrapper_art__Assns_recob__PFParticle_larpandoraobj__PFParticleMetadata_void____cxx
#define art__Wrapper_art__Assns_recob__PFParticle_larpandoraobj__PFParticleMetadata_void____cxx
art::Wrapper<art::Assns<recob::PFParticle,larpandoraobj::PFParticleMetadata,void> >::Wrapper() {
}
art::Wrapper<art::Assns<recob::PFParticle,larpandoraobj::PFParticleMetadata,void> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
art::Wrapper<art::Assns<recob::PFParticle,larpandoraobj::PFParticleMetadata,void> >::~Wrapper() {
}
#endif // art__Wrapper_art__Assns_recob__PFParticle_larpandoraobj__PFParticleMetadata_void____cxx

#ifndef art__Assns_recob__PFParticle_larpandoraobj__PFParticleMetadata_void__cxx
#define art__Assns_recob__PFParticle_larpandoraobj__PFParticleMetadata_void__cxx
art::Assns<recob::PFParticle,larpandoraobj::PFParticleMetadata,void>::Assns() {
}
art::Assns<recob::PFParticle,larpandoraobj::PFParticleMetadata,void>::Assns(const Assns & rhs)
   : art::detail::AssnsBase(const_cast<Assns &>( rhs ))
   , ptr_data_1_(const_cast<Assns &>( rhs ).ptr_data_1_)
   , ptr_data_2_(const_cast<Assns &>( rhs ).ptr_data_2_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Assns &modrhs = const_cast<Assns &>( rhs );
   modrhs.ptr_data_1_.clear();
   modrhs.ptr_data_2_.clear();
}
art::Assns<recob::PFParticle,larpandoraobj::PFParticleMetadata,void>::~Assns() {
}
#endif // art__Assns_recob__PFParticle_larpandoraobj__PFParticleMetadata_void__cxx

#ifndef art__Wrapper_art__Assns_recob__PFParticle_recob__Cluster_void____cxx
#define art__Wrapper_art__Assns_recob__PFParticle_recob__Cluster_void____cxx
art::Wrapper<art::Assns<recob::PFParticle,recob::Cluster,void> >::Wrapper() {
}
art::Wrapper<art::Assns<recob::PFParticle,recob::Cluster,void> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
art::Wrapper<art::Assns<recob::PFParticle,recob::Cluster,void> >::~Wrapper() {
}
#endif // art__Wrapper_art__Assns_recob__PFParticle_recob__Cluster_void____cxx

#ifndef art__Assns_recob__PFParticle_recob__Cluster_void__cxx
#define art__Assns_recob__PFParticle_recob__Cluster_void__cxx
art::Assns<recob::PFParticle,recob::Cluster,void>::Assns() {
}
art::Assns<recob::PFParticle,recob::Cluster,void>::Assns(const Assns & rhs)
   : art::detail::AssnsBase(const_cast<Assns &>( rhs ))
   , ptr_data_1_(const_cast<Assns &>( rhs ).ptr_data_1_)
   , ptr_data_2_(const_cast<Assns &>( rhs ).ptr_data_2_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Assns &modrhs = const_cast<Assns &>( rhs );
   modrhs.ptr_data_1_.clear();
   modrhs.ptr_data_2_.clear();
}
art::Assns<recob::PFParticle,recob::Cluster,void>::~Assns() {
}
#endif // art__Assns_recob__PFParticle_recob__Cluster_void__cxx

#ifndef art__Wrapper_art__Assns_recob__PFParticle_recob__PCAxis_void____cxx
#define art__Wrapper_art__Assns_recob__PFParticle_recob__PCAxis_void____cxx
art::Wrapper<art::Assns<recob::PFParticle,recob::PCAxis,void> >::Wrapper() {
}
art::Wrapper<art::Assns<recob::PFParticle,recob::PCAxis,void> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
art::Wrapper<art::Assns<recob::PFParticle,recob::PCAxis,void> >::~Wrapper() {
}
#endif // art__Wrapper_art__Assns_recob__PFParticle_recob__PCAxis_void____cxx

#ifndef art__Assns_recob__PFParticle_recob__PCAxis_void__cxx
#define art__Assns_recob__PFParticle_recob__PCAxis_void__cxx
art::Assns<recob::PFParticle,recob::PCAxis,void>::Assns() {
}
art::Assns<recob::PFParticle,recob::PCAxis,void>::Assns(const Assns & rhs)
   : art::detail::AssnsBase(const_cast<Assns &>( rhs ))
   , ptr_data_1_(const_cast<Assns &>( rhs ).ptr_data_1_)
   , ptr_data_2_(const_cast<Assns &>( rhs ).ptr_data_2_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Assns &modrhs = const_cast<Assns &>( rhs );
   modrhs.ptr_data_1_.clear();
   modrhs.ptr_data_2_.clear();
}
art::Assns<recob::PFParticle,recob::PCAxis,void>::~Assns() {
}
#endif // art__Assns_recob__PFParticle_recob__PCAxis_void__cxx

#ifndef art__Wrapper_art__Assns_recob__PFParticle_recob__Shower_void____cxx
#define art__Wrapper_art__Assns_recob__PFParticle_recob__Shower_void____cxx
art::Wrapper<art::Assns<recob::PFParticle,recob::Shower,void> >::Wrapper() {
}
art::Wrapper<art::Assns<recob::PFParticle,recob::Shower,void> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
art::Wrapper<art::Assns<recob::PFParticle,recob::Shower,void> >::~Wrapper() {
}
#endif // art__Wrapper_art__Assns_recob__PFParticle_recob__Shower_void____cxx

#ifndef art__Assns_recob__PFParticle_recob__Shower_void__cxx
#define art__Assns_recob__PFParticle_recob__Shower_void__cxx
art::Assns<recob::PFParticle,recob::Shower,void>::Assns() {
}
art::Assns<recob::PFParticle,recob::Shower,void>::Assns(const Assns & rhs)
   : art::detail::AssnsBase(const_cast<Assns &>( rhs ))
   , ptr_data_1_(const_cast<Assns &>( rhs ).ptr_data_1_)
   , ptr_data_2_(const_cast<Assns &>( rhs ).ptr_data_2_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Assns &modrhs = const_cast<Assns &>( rhs );
   modrhs.ptr_data_1_.clear();
   modrhs.ptr_data_2_.clear();
}
art::Assns<recob::PFParticle,recob::Shower,void>::~Assns() {
}
#endif // art__Assns_recob__PFParticle_recob__Shower_void__cxx

#ifndef art__Wrapper_art__Assns_recob__PFParticle_recob__SpacePoint_void____cxx
#define art__Wrapper_art__Assns_recob__PFParticle_recob__SpacePoint_void____cxx
art::Wrapper<art::Assns<recob::PFParticle,recob::SpacePoint,void> >::Wrapper() {
}
art::Wrapper<art::Assns<recob::PFParticle,recob::SpacePoint,void> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
art::Wrapper<art::Assns<recob::PFParticle,recob::SpacePoint,void> >::~Wrapper() {
}
#endif // art__Wrapper_art__Assns_recob__PFParticle_recob__SpacePoint_void____cxx

#ifndef art__Assns_recob__PFParticle_recob__SpacePoint_void__cxx
#define art__Assns_recob__PFParticle_recob__SpacePoint_void__cxx
art::Assns<recob::PFParticle,recob::SpacePoint,void>::Assns() {
}
art::Assns<recob::PFParticle,recob::SpacePoint,void>::Assns(const Assns & rhs)
   : art::detail::AssnsBase(const_cast<Assns &>( rhs ))
   , ptr_data_1_(const_cast<Assns &>( rhs ).ptr_data_1_)
   , ptr_data_2_(const_cast<Assns &>( rhs ).ptr_data_2_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Assns &modrhs = const_cast<Assns &>( rhs );
   modrhs.ptr_data_1_.clear();
   modrhs.ptr_data_2_.clear();
}
art::Assns<recob::PFParticle,recob::SpacePoint,void>::~Assns() {
}
#endif // art__Assns_recob__PFParticle_recob__SpacePoint_void__cxx

#ifndef art__Wrapper_art__Assns_recob__PFParticle_recob__Track_void____cxx
#define art__Wrapper_art__Assns_recob__PFParticle_recob__Track_void____cxx
art::Wrapper<art::Assns<recob::PFParticle,recob::Track,void> >::Wrapper() {
}
art::Wrapper<art::Assns<recob::PFParticle,recob::Track,void> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
art::Wrapper<art::Assns<recob::PFParticle,recob::Track,void> >::~Wrapper() {
}
#endif // art__Wrapper_art__Assns_recob__PFParticle_recob__Track_void____cxx

#ifndef art__Assns_recob__PFParticle_recob__Track_void__cxx
#define art__Assns_recob__PFParticle_recob__Track_void__cxx
art::Assns<recob::PFParticle,recob::Track,void>::Assns() {
}
art::Assns<recob::PFParticle,recob::Track,void>::Assns(const Assns & rhs)
   : art::detail::AssnsBase(const_cast<Assns &>( rhs ))
   , ptr_data_1_(const_cast<Assns &>( rhs ).ptr_data_1_)
   , ptr_data_2_(const_cast<Assns &>( rhs ).ptr_data_2_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Assns &modrhs = const_cast<Assns &>( rhs );
   modrhs.ptr_data_1_.clear();
   modrhs.ptr_data_2_.clear();
}
art::Assns<recob::PFParticle,recob::Track,void>::~Assns() {
}
#endif // art__Assns_recob__PFParticle_recob__Track_void__cxx

#ifndef art__Wrapper_art__Assns_recob__PFParticle_recob__Vertex_void____cxx
#define art__Wrapper_art__Assns_recob__PFParticle_recob__Vertex_void____cxx
art::Wrapper<art::Assns<recob::PFParticle,recob::Vertex,void> >::Wrapper() {
}
art::Wrapper<art::Assns<recob::PFParticle,recob::Vertex,void> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
art::Wrapper<art::Assns<recob::PFParticle,recob::Vertex,void> >::~Wrapper() {
}
#endif // art__Wrapper_art__Assns_recob__PFParticle_recob__Vertex_void____cxx

#ifndef art__Assns_recob__PFParticle_recob__Vertex_void__cxx
#define art__Assns_recob__PFParticle_recob__Vertex_void__cxx
art::Assns<recob::PFParticle,recob::Vertex,void>::Assns() {
}
art::Assns<recob::PFParticle,recob::Vertex,void>::Assns(const Assns & rhs)
   : art::detail::AssnsBase(const_cast<Assns &>( rhs ))
   , ptr_data_1_(const_cast<Assns &>( rhs ).ptr_data_1_)
   , ptr_data_2_(const_cast<Assns &>( rhs ).ptr_data_2_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Assns &modrhs = const_cast<Assns &>( rhs );
   modrhs.ptr_data_1_.clear();
   modrhs.ptr_data_2_.clear();
}
art::Assns<recob::PFParticle,recob::Vertex,void>::~Assns() {
}
#endif // art__Assns_recob__PFParticle_recob__Vertex_void__cxx

#ifndef art__Wrapper_art__Assns_recob__Shower_recob__Hit_void____cxx
#define art__Wrapper_art__Assns_recob__Shower_recob__Hit_void____cxx
art::Wrapper<art::Assns<recob::Shower,recob::Hit,void> >::Wrapper() {
}
art::Wrapper<art::Assns<recob::Shower,recob::Hit,void> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
art::Wrapper<art::Assns<recob::Shower,recob::Hit,void> >::~Wrapper() {
}
#endif // art__Wrapper_art__Assns_recob__Shower_recob__Hit_void____cxx

#ifndef art__Assns_recob__Shower_recob__Hit_void__cxx
#define art__Assns_recob__Shower_recob__Hit_void__cxx
art::Assns<recob::Shower,recob::Hit,void>::Assns() {
}
art::Assns<recob::Shower,recob::Hit,void>::Assns(const Assns & rhs)
   : art::detail::AssnsBase(const_cast<Assns &>( rhs ))
   , ptr_data_1_(const_cast<Assns &>( rhs ).ptr_data_1_)
   , ptr_data_2_(const_cast<Assns &>( rhs ).ptr_data_2_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Assns &modrhs = const_cast<Assns &>( rhs );
   modrhs.ptr_data_1_.clear();
   modrhs.ptr_data_2_.clear();
}
art::Assns<recob::Shower,recob::Hit,void>::~Assns() {
}
#endif // art__Assns_recob__Shower_recob__Hit_void__cxx

#ifndef art__Wrapper_art__Assns_recob__Shower_recob__PCAxis_void____cxx
#define art__Wrapper_art__Assns_recob__Shower_recob__PCAxis_void____cxx
art::Wrapper<art::Assns<recob::Shower,recob::PCAxis,void> >::Wrapper() {
}
art::Wrapper<art::Assns<recob::Shower,recob::PCAxis,void> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
art::Wrapper<art::Assns<recob::Shower,recob::PCAxis,void> >::~Wrapper() {
}
#endif // art__Wrapper_art__Assns_recob__Shower_recob__PCAxis_void____cxx

#ifndef art__Assns_recob__Shower_recob__PCAxis_void__cxx
#define art__Assns_recob__Shower_recob__PCAxis_void__cxx
art::Assns<recob::Shower,recob::PCAxis,void>::Assns() {
}
art::Assns<recob::Shower,recob::PCAxis,void>::Assns(const Assns & rhs)
   : art::detail::AssnsBase(const_cast<Assns &>( rhs ))
   , ptr_data_1_(const_cast<Assns &>( rhs ).ptr_data_1_)
   , ptr_data_2_(const_cast<Assns &>( rhs ).ptr_data_2_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Assns &modrhs = const_cast<Assns &>( rhs );
   modrhs.ptr_data_1_.clear();
   modrhs.ptr_data_2_.clear();
}
art::Assns<recob::Shower,recob::PCAxis,void>::~Assns() {
}
#endif // art__Assns_recob__Shower_recob__PCAxis_void__cxx

#ifndef art__Wrapper_art__Assns_recob__SpacePoint_recob__Hit_void____cxx
#define art__Wrapper_art__Assns_recob__SpacePoint_recob__Hit_void____cxx
art::Wrapper<art::Assns<recob::SpacePoint,recob::Hit,void> >::Wrapper() {
}
art::Wrapper<art::Assns<recob::SpacePoint,recob::Hit,void> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
art::Wrapper<art::Assns<recob::SpacePoint,recob::Hit,void> >::~Wrapper() {
}
#endif // art__Wrapper_art__Assns_recob__SpacePoint_recob__Hit_void____cxx

#ifndef art__Assns_recob__SpacePoint_recob__Hit_void__cxx
#define art__Assns_recob__SpacePoint_recob__Hit_void__cxx
art::Assns<recob::SpacePoint,recob::Hit,void>::Assns() {
}
art::Assns<recob::SpacePoint,recob::Hit,void>::Assns(const Assns & rhs)
   : art::detail::AssnsBase(const_cast<Assns &>( rhs ))
   , ptr_data_1_(const_cast<Assns &>( rhs ).ptr_data_1_)
   , ptr_data_2_(const_cast<Assns &>( rhs ).ptr_data_2_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Assns &modrhs = const_cast<Assns &>( rhs );
   modrhs.ptr_data_1_.clear();
   modrhs.ptr_data_2_.clear();
}
art::Assns<recob::SpacePoint,recob::Hit,void>::~Assns() {
}
#endif // art__Assns_recob__SpacePoint_recob__Hit_void__cxx

#ifndef art__Wrapper_art__Assns_recob__Track_anab__Calorimetry_void____cxx
#define art__Wrapper_art__Assns_recob__Track_anab__Calorimetry_void____cxx
art::Wrapper<art::Assns<recob::Track,anab::Calorimetry,void> >::Wrapper() {
}
art::Wrapper<art::Assns<recob::Track,anab::Calorimetry,void> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
art::Wrapper<art::Assns<recob::Track,anab::Calorimetry,void> >::~Wrapper() {
}
#endif // art__Wrapper_art__Assns_recob__Track_anab__Calorimetry_void____cxx

#ifndef art__Assns_recob__Track_anab__Calorimetry_void__cxx
#define art__Assns_recob__Track_anab__Calorimetry_void__cxx
art::Assns<recob::Track,anab::Calorimetry,void>::Assns() {
}
art::Assns<recob::Track,anab::Calorimetry,void>::Assns(const Assns & rhs)
   : art::detail::AssnsBase(const_cast<Assns &>( rhs ))
   , ptr_data_1_(const_cast<Assns &>( rhs ).ptr_data_1_)
   , ptr_data_2_(const_cast<Assns &>( rhs ).ptr_data_2_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Assns &modrhs = const_cast<Assns &>( rhs );
   modrhs.ptr_data_1_.clear();
   modrhs.ptr_data_2_.clear();
}
art::Assns<recob::Track,anab::Calorimetry,void>::~Assns() {
}
#endif // art__Assns_recob__Track_anab__Calorimetry_void__cxx

#ifndef art__Wrapper_art__Assns_recob__Track_anab__CosmicTag_void____cxx
#define art__Wrapper_art__Assns_recob__Track_anab__CosmicTag_void____cxx
art::Wrapper<art::Assns<recob::Track,anab::CosmicTag,void> >::Wrapper() {
}
art::Wrapper<art::Assns<recob::Track,anab::CosmicTag,void> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
art::Wrapper<art::Assns<recob::Track,anab::CosmicTag,void> >::~Wrapper() {
}
#endif // art__Wrapper_art__Assns_recob__Track_anab__CosmicTag_void____cxx

#ifndef art__Assns_recob__Track_anab__CosmicTag_void__cxx
#define art__Assns_recob__Track_anab__CosmicTag_void__cxx
art::Assns<recob::Track,anab::CosmicTag,void>::Assns() {
}
art::Assns<recob::Track,anab::CosmicTag,void>::Assns(const Assns & rhs)
   : art::detail::AssnsBase(const_cast<Assns &>( rhs ))
   , ptr_data_1_(const_cast<Assns &>( rhs ).ptr_data_1_)
   , ptr_data_2_(const_cast<Assns &>( rhs ).ptr_data_2_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Assns &modrhs = const_cast<Assns &>( rhs );
   modrhs.ptr_data_1_.clear();
   modrhs.ptr_data_2_.clear();
}
art::Assns<recob::Track,anab::CosmicTag,void>::~Assns() {
}
#endif // art__Assns_recob__Track_anab__CosmicTag_void__cxx

#ifndef art__Wrapper_art__Assns_recob__Track_anab__ParticleID_void____cxx
#define art__Wrapper_art__Assns_recob__Track_anab__ParticleID_void____cxx
art::Wrapper<art::Assns<recob::Track,anab::ParticleID,void> >::Wrapper() {
}
art::Wrapper<art::Assns<recob::Track,anab::ParticleID,void> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
art::Wrapper<art::Assns<recob::Track,anab::ParticleID,void> >::~Wrapper() {
}
#endif // art__Wrapper_art__Assns_recob__Track_anab__ParticleID_void____cxx

#ifndef art__Assns_recob__Track_anab__ParticleID_void__cxx
#define art__Assns_recob__Track_anab__ParticleID_void__cxx
art::Assns<recob::Track,anab::ParticleID,void>::Assns() {
}
art::Assns<recob::Track,anab::ParticleID,void>::Assns(const Assns & rhs)
   : art::detail::AssnsBase(const_cast<Assns &>( rhs ))
   , ptr_data_1_(const_cast<Assns &>( rhs ).ptr_data_1_)
   , ptr_data_2_(const_cast<Assns &>( rhs ).ptr_data_2_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Assns &modrhs = const_cast<Assns &>( rhs );
   modrhs.ptr_data_1_.clear();
   modrhs.ptr_data_2_.clear();
}
art::Assns<recob::Track,anab::ParticleID,void>::~Assns() {
}
#endif // art__Assns_recob__Track_anab__ParticleID_void__cxx

#ifndef art__Wrapper_art__Assns_recob__Track_anab__T0_void____cxx
#define art__Wrapper_art__Assns_recob__Track_anab__T0_void____cxx
art::Wrapper<art::Assns<recob::Track,anab::T0,void> >::Wrapper() {
}
art::Wrapper<art::Assns<recob::Track,anab::T0,void> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
art::Wrapper<art::Assns<recob::Track,anab::T0,void> >::~Wrapper() {
}
#endif // art__Wrapper_art__Assns_recob__Track_anab__T0_void____cxx

#ifndef art__Assns_recob__Track_anab__T0_void__cxx
#define art__Assns_recob__Track_anab__T0_void__cxx
art::Assns<recob::Track,anab::T0,void>::Assns() {
}
art::Assns<recob::Track,anab::T0,void>::Assns(const Assns & rhs)
   : art::detail::AssnsBase(const_cast<Assns &>( rhs ))
   , ptr_data_1_(const_cast<Assns &>( rhs ).ptr_data_1_)
   , ptr_data_2_(const_cast<Assns &>( rhs ).ptr_data_2_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Assns &modrhs = const_cast<Assns &>( rhs );
   modrhs.ptr_data_1_.clear();
   modrhs.ptr_data_2_.clear();
}
art::Assns<recob::Track,anab::T0,void>::~Assns() {
}
#endif // art__Assns_recob__Track_anab__T0_void__cxx

#ifndef art__Wrapper_art__Assns_recob__Track_recob__Hit_recob__TrackHitMeta____cxx
#define art__Wrapper_art__Assns_recob__Track_recob__Hit_recob__TrackHitMeta____cxx
art::Wrapper<art::Assns<recob::Track,recob::Hit,recob::TrackHitMeta> >::Wrapper() {
}
art::Wrapper<art::Assns<recob::Track,recob::Hit,recob::TrackHitMeta> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
art::Wrapper<art::Assns<recob::Track,recob::Hit,recob::TrackHitMeta> >::~Wrapper() {
}
#endif // art__Wrapper_art__Assns_recob__Track_recob__Hit_recob__TrackHitMeta____cxx

#ifndef art__Assns_recob__Track_recob__Hit_recob__TrackHitMeta__cxx
#define art__Assns_recob__Track_recob__Hit_recob__TrackHitMeta__cxx
art::Assns<recob::Track,recob::Hit,recob::TrackHitMeta>::Assns() {
}
art::Assns<recob::Track,recob::Hit,recob::TrackHitMeta>::Assns(const Assns & rhs)
   : art::Assns<recob::Track,recob::Hit,void>(const_cast<Assns &>( rhs ))
   , data_(const_cast<Assns &>( rhs ).data_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Assns &modrhs = const_cast<Assns &>( rhs );
   modrhs.data_.clear();
}
art::Assns<recob::Track,recob::Hit,recob::TrackHitMeta>::~Assns() {
}
#endif // art__Assns_recob__Track_recob__Hit_recob__TrackHitMeta__cxx

#ifndef art__Assns_recob__Track_recob__Hit_void__cxx
#define art__Assns_recob__Track_recob__Hit_void__cxx
art::Assns<recob::Track,recob::Hit,void>::Assns() {
}
art::Assns<recob::Track,recob::Hit,void>::Assns(const Assns & rhs)
   : art::detail::AssnsBase(const_cast<Assns &>( rhs ))
   , ptr_data_1_(const_cast<Assns &>( rhs ).ptr_data_1_)
   , ptr_data_2_(const_cast<Assns &>( rhs ).ptr_data_2_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Assns &modrhs = const_cast<Assns &>( rhs );
   modrhs.ptr_data_1_.clear();
   modrhs.ptr_data_2_.clear();
}
art::Assns<recob::Track,recob::Hit,void>::~Assns() {
}
#endif // art__Assns_recob__Track_recob__Hit_void__cxx

#ifndef art__Wrapper_art__Assns_recob__Track_recob__Hit_void____cxx
#define art__Wrapper_art__Assns_recob__Track_recob__Hit_void____cxx
art::Wrapper<art::Assns<recob::Track,recob::Hit,void> >::Wrapper() {
}
art::Wrapper<art::Assns<recob::Track,recob::Hit,void> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
art::Wrapper<art::Assns<recob::Track,recob::Hit,void> >::~Wrapper() {
}
#endif // art__Wrapper_art__Assns_recob__Track_recob__Hit_void____cxx

#ifndef art__Wrapper_art__Assns_recob__Track_recob__SpacePoint_void____cxx
#define art__Wrapper_art__Assns_recob__Track_recob__SpacePoint_void____cxx
art::Wrapper<art::Assns<recob::Track,recob::SpacePoint,void> >::Wrapper() {
}
art::Wrapper<art::Assns<recob::Track,recob::SpacePoint,void> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
art::Wrapper<art::Assns<recob::Track,recob::SpacePoint,void> >::~Wrapper() {
}
#endif // art__Wrapper_art__Assns_recob__Track_recob__SpacePoint_void____cxx

#ifndef art__Assns_recob__Track_recob__SpacePoint_void__cxx
#define art__Assns_recob__Track_recob__SpacePoint_void__cxx
art::Assns<recob::Track,recob::SpacePoint,void>::Assns() {
}
art::Assns<recob::Track,recob::SpacePoint,void>::Assns(const Assns & rhs)
   : art::detail::AssnsBase(const_cast<Assns &>( rhs ))
   , ptr_data_1_(const_cast<Assns &>( rhs ).ptr_data_1_)
   , ptr_data_2_(const_cast<Assns &>( rhs ).ptr_data_2_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Assns &modrhs = const_cast<Assns &>( rhs );
   modrhs.ptr_data_1_.clear();
   modrhs.ptr_data_2_.clear();
}
art::Assns<recob::Track,recob::SpacePoint,void>::~Assns() {
}
#endif // art__Assns_recob__Track_recob__SpacePoint_void__cxx

#ifndef art__Wrapper_art__Assns_recob__Track_recob__Vertex_void____cxx
#define art__Wrapper_art__Assns_recob__Track_recob__Vertex_void____cxx
art::Wrapper<art::Assns<recob::Track,recob::Vertex,void> >::Wrapper() {
}
art::Wrapper<art::Assns<recob::Track,recob::Vertex,void> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
art::Wrapper<art::Assns<recob::Track,recob::Vertex,void> >::~Wrapper() {
}
#endif // art__Wrapper_art__Assns_recob__Track_recob__Vertex_void____cxx

#ifndef art__Assns_recob__Track_recob__Vertex_void__cxx
#define art__Assns_recob__Track_recob__Vertex_void__cxx
art::Assns<recob::Track,recob::Vertex,void>::Assns() {
}
art::Assns<recob::Track,recob::Vertex,void>::Assns(const Assns & rhs)
   : art::detail::AssnsBase(const_cast<Assns &>( rhs ))
   , ptr_data_1_(const_cast<Assns &>( rhs ).ptr_data_1_)
   , ptr_data_2_(const_cast<Assns &>( rhs ).ptr_data_2_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Assns &modrhs = const_cast<Assns &>( rhs );
   modrhs.ptr_data_1_.clear();
   modrhs.ptr_data_2_.clear();
}
art::Assns<recob::Track,recob::Vertex,void>::~Assns() {
}
#endif // art__Assns_recob__Track_recob__Vertex_void__cxx

#ifndef art__Wrapper_art__Assns_recob__Vertex_recob__Track_void____cxx
#define art__Wrapper_art__Assns_recob__Vertex_recob__Track_void____cxx
art::Wrapper<art::Assns<recob::Vertex,recob::Track,void> >::Wrapper() {
}
art::Wrapper<art::Assns<recob::Vertex,recob::Track,void> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
art::Wrapper<art::Assns<recob::Vertex,recob::Track,void> >::~Wrapper() {
}
#endif // art__Wrapper_art__Assns_recob__Vertex_recob__Track_void____cxx

#ifndef art__Assns_recob__Vertex_recob__Track_void__cxx
#define art__Assns_recob__Vertex_recob__Track_void__cxx
art::Assns<recob::Vertex,recob::Track,void>::Assns() {
}
art::Assns<recob::Vertex,recob::Track,void>::Assns(const Assns & rhs)
   : art::detail::AssnsBase(const_cast<Assns &>( rhs ))
   , ptr_data_1_(const_cast<Assns &>( rhs ).ptr_data_1_)
   , ptr_data_2_(const_cast<Assns &>( rhs ).ptr_data_2_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Assns &modrhs = const_cast<Assns &>( rhs );
   modrhs.ptr_data_1_.clear();
   modrhs.ptr_data_2_.clear();
}
art::Assns<recob::Vertex,recob::Track,void>::~Assns() {
}
#endif // art__Assns_recob__Vertex_recob__Track_void__cxx

#ifndef art__Wrapper_art__Assns_recob__Wire_recob__Hit_void____cxx
#define art__Wrapper_art__Assns_recob__Wire_recob__Hit_void____cxx
art::Wrapper<art::Assns<recob::Wire,recob::Hit,void> >::Wrapper() {
}
art::Wrapper<art::Assns<recob::Wire,recob::Hit,void> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
art::Wrapper<art::Assns<recob::Wire,recob::Hit,void> >::~Wrapper() {
}
#endif // art__Wrapper_art__Assns_recob__Wire_recob__Hit_void____cxx

#ifndef art__Assns_recob__Wire_recob__Hit_void__cxx
#define art__Assns_recob__Wire_recob__Hit_void__cxx
art::Assns<recob::Wire,recob::Hit,void>::Assns() {
}
art::Assns<recob::Wire,recob::Hit,void>::Assns(const Assns & rhs)
   : art::detail::AssnsBase(const_cast<Assns &>( rhs ))
   , ptr_data_1_(const_cast<Assns &>( rhs ).ptr_data_1_)
   , ptr_data_2_(const_cast<Assns &>( rhs ).ptr_data_2_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Assns &modrhs = const_cast<Assns &>( rhs );
   modrhs.ptr_data_1_.clear();
   modrhs.ptr_data_2_.clear();
}
art::Assns<recob::Wire,recob::Hit,void>::~Assns() {
}
#endif // art__Assns_recob__Wire_recob__Hit_void__cxx

#ifndef art__Wrapper_vector_anab__Calorimetry____cxx
#define art__Wrapper_vector_anab__Calorimetry____cxx
art::Wrapper<vector<anab::Calorimetry> >::Wrapper() {
}
art::Wrapper<vector<anab::Calorimetry> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Wrapper &modrhs = const_cast<Wrapper &>( rhs );
   modrhs.obj.clear();
}
art::Wrapper<vector<anab::Calorimetry> >::~Wrapper() {
}
#endif // art__Wrapper_vector_anab__Calorimetry____cxx

#ifndef anab__Calorimetry_cxx
#define anab__Calorimetry_cxx
anab::Calorimetry::Calorimetry() {
}
anab::Calorimetry::Calorimetry(const Calorimetry & rhs)
   : fKineticEnergy(const_cast<Calorimetry &>( rhs ).fKineticEnergy)
   , fdEdx(const_cast<Calorimetry &>( rhs ).fdEdx)
   , fdQdx(const_cast<Calorimetry &>( rhs ).fdQdx)
   , fResidualRange(const_cast<Calorimetry &>( rhs ).fResidualRange)
   , fDeadWireResR(const_cast<Calorimetry &>( rhs ).fDeadWireResR)
   , fRange(const_cast<Calorimetry &>( rhs ).fRange)
   , fTrkPitch(const_cast<Calorimetry &>( rhs ).fTrkPitch)
   , fXYZ(const_cast<Calorimetry &>( rhs ).fXYZ)
   , fPlaneID(const_cast<Calorimetry &>( rhs ).fPlaneID)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Calorimetry &modrhs = const_cast<Calorimetry &>( rhs );
   modrhs.fdEdx.clear();
   modrhs.fdQdx.clear();
   modrhs.fResidualRange.clear();
   modrhs.fDeadWireResR.clear();
   modrhs.fTrkPitch.clear();
   modrhs.fXYZ.clear();
}
anab::Calorimetry::~Calorimetry() {
}
#endif // anab__Calorimetry_cxx

#ifndef geo__PlaneID_cxx
#define geo__PlaneID_cxx
geo::PlaneID::PlaneID() {
}
geo::PlaneID::PlaneID(const PlaneID & rhs)
   : geo::TPCID(const_cast<PlaneID &>( rhs ))
   , Plane(const_cast<PlaneID &>( rhs ).Plane)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
geo::PlaneID::~PlaneID() {
}
#endif // geo__PlaneID_cxx

#ifndef geo__TPCID_cxx
#define geo__TPCID_cxx
geo::TPCID::TPCID() {
}
geo::TPCID::TPCID(const TPCID & rhs)
   : geo::CryostatID(const_cast<TPCID &>( rhs ))
   , TPC(const_cast<TPCID &>( rhs ).TPC)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
geo::TPCID::~TPCID() {
}
#endif // geo__TPCID_cxx

#ifndef geo__CryostatID_cxx
#define geo__CryostatID_cxx
geo::CryostatID::CryostatID() {
}
geo::CryostatID::CryostatID(const CryostatID & rhs)
   : isValid(const_cast<CryostatID &>( rhs ).isValid)
   , Cryostat(const_cast<CryostatID &>( rhs ).Cryostat)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
geo::CryostatID::~CryostatID() {
}
#endif // geo__CryostatID_cxx

#ifndef art__Wrapper_vector_anab__CosmicTag____cxx
#define art__Wrapper_vector_anab__CosmicTag____cxx
art::Wrapper<vector<anab::CosmicTag> >::Wrapper() {
}
art::Wrapper<vector<anab::CosmicTag> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Wrapper &modrhs = const_cast<Wrapper &>( rhs );
   modrhs.obj.clear();
}
art::Wrapper<vector<anab::CosmicTag> >::~Wrapper() {
}
#endif // art__Wrapper_vector_anab__CosmicTag____cxx

#ifndef anab__CosmicTag_cxx
#define anab__CosmicTag_cxx
anab::CosmicTag::CosmicTag() {
}
anab::CosmicTag::CosmicTag(const CosmicTag & rhs)
   : endPt1(const_cast<CosmicTag &>( rhs ).endPt1)
   , endPt2(const_cast<CosmicTag &>( rhs ).endPt2)
   , fCosmicScore(const_cast<CosmicTag &>( rhs ).fCosmicScore)
   , fCosmicType(const_cast<CosmicTag &>( rhs ).fCosmicType)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   CosmicTag &modrhs = const_cast<CosmicTag &>( rhs );
   modrhs.endPt1.clear();
   modrhs.endPt2.clear();
}
anab::CosmicTag::~CosmicTag() {
}
#endif // anab__CosmicTag_cxx

#ifndef art__Wrapper_vector_anab__ParticleID____cxx
#define art__Wrapper_vector_anab__ParticleID____cxx
art::Wrapper<vector<anab::ParticleID> >::Wrapper() {
}
art::Wrapper<vector<anab::ParticleID> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Wrapper &modrhs = const_cast<Wrapper &>( rhs );
   modrhs.obj.clear();
}
art::Wrapper<vector<anab::ParticleID> >::~Wrapper() {
}
#endif // art__Wrapper_vector_anab__ParticleID____cxx

#ifndef anab__ParticleID_cxx
#define anab__ParticleID_cxx
anab::ParticleID::ParticleID() {
}
anab::ParticleID::ParticleID(const ParticleID & rhs)
   : fPdg(const_cast<ParticleID &>( rhs ).fPdg)
   , fNdf(const_cast<ParticleID &>( rhs ).fNdf)
   , fMinChi2(const_cast<ParticleID &>( rhs ).fMinChi2)
   , fDeltaChi2(const_cast<ParticleID &>( rhs ).fDeltaChi2)
   , fChi2Proton(const_cast<ParticleID &>( rhs ).fChi2Proton)
   , fChi2Kaon(const_cast<ParticleID &>( rhs ).fChi2Kaon)
   , fChi2Pion(const_cast<ParticleID &>( rhs ).fChi2Pion)
   , fChi2Muon(const_cast<ParticleID &>( rhs ).fChi2Muon)
   , fMissingE(const_cast<ParticleID &>( rhs ).fMissingE)
   , fMissingEavg(const_cast<ParticleID &>( rhs ).fMissingEavg)
   , fPIDA(const_cast<ParticleID &>( rhs ).fPIDA)
   , fPlaneID(const_cast<ParticleID &>( rhs ).fPlaneID)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
anab::ParticleID::~ParticleID() {
}
#endif // anab__ParticleID_cxx

#ifndef art__Wrapper_vector_anab__T0____cxx
#define art__Wrapper_vector_anab__T0____cxx
art::Wrapper<vector<anab::T0> >::Wrapper() {
}
art::Wrapper<vector<anab::T0> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Wrapper &modrhs = const_cast<Wrapper &>( rhs );
   modrhs.obj.clear();
}
art::Wrapper<vector<anab::T0> >::~Wrapper() {
}
#endif // art__Wrapper_vector_anab__T0____cxx

#ifndef anab__T0_cxx
#define anab__T0_cxx
anab::T0::T0() {
}
anab::T0::T0(const T0 & rhs)
   : fTime(const_cast<T0 &>( rhs ).fTime)
   , fTriggerType(const_cast<T0 &>( rhs ).fTriggerType)
   , fTriggerBits(const_cast<T0 &>( rhs ).fTriggerBits)
   , fID(const_cast<T0 &>( rhs ).fID)
   , fTriggerConfidence(const_cast<T0 &>( rhs ).fTriggerConfidence)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
anab::T0::~T0() {
}
#endif // anab__T0_cxx

#ifndef art__Wrapper_vector_beam__ProtoDUNEBeamEvent____cxx
#define art__Wrapper_vector_beam__ProtoDUNEBeamEvent____cxx
art::Wrapper<vector<beam::ProtoDUNEBeamEvent> >::Wrapper() {
}
art::Wrapper<vector<beam::ProtoDUNEBeamEvent> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Wrapper &modrhs = const_cast<Wrapper &>( rhs );
   modrhs.obj.clear();
}
art::Wrapper<vector<beam::ProtoDUNEBeamEvent> >::~Wrapper() {
}
#endif // art__Wrapper_vector_beam__ProtoDUNEBeamEvent____cxx

#ifndef beam__ProtoDUNEBeamEvent_cxx
#define beam__ProtoDUNEBeamEvent_cxx
beam::ProtoDUNEBeamEvent::ProtoDUNEBeamEvent() {
}
beam::ProtoDUNEBeamEvent::ProtoDUNEBeamEvent(const ProtoDUNEBeamEvent & rhs)
   : t0(const_cast<ProtoDUNEBeamEvent &>( rhs ).t0)
   , CTBTimestamp(const_cast<ProtoDUNEBeamEvent &>( rhs ).CTBTimestamp)
   , RDTimestamp(const_cast<ProtoDUNEBeamEvent &>( rhs ).RDTimestamp)
   , fiberMonitors(const_cast<ProtoDUNEBeamEvent &>( rhs ).fiberMonitors)
   , TOF0(const_cast<ProtoDUNEBeamEvent &>( rhs ).TOF0)
   , TOF1(const_cast<ProtoDUNEBeamEvent &>( rhs ).TOF1)
   , TOFChan(const_cast<ProtoDUNEBeamEvent &>( rhs ).TOFChan)
   , theTOF(const_cast<ProtoDUNEBeamEvent &>( rhs ).theTOF)
   , CKov0(const_cast<ProtoDUNEBeamEvent &>( rhs ).CKov0)
   , CKov1(const_cast<ProtoDUNEBeamEvent &>( rhs ).CKov1)
   , Tracks(const_cast<ProtoDUNEBeamEvent &>( rhs ).Tracks)
   , activeTrigger(const_cast<ProtoDUNEBeamEvent &>( rhs ).activeTrigger)
   , isMatched(const_cast<ProtoDUNEBeamEvent &>( rhs ).isMatched)
   , RecoBeamMomenta(const_cast<ProtoDUNEBeamEvent &>( rhs ).RecoBeamMomenta)
   , BITrigger(const_cast<ProtoDUNEBeamEvent &>( rhs ).BITrigger)
   , TimingTrigger(const_cast<ProtoDUNEBeamEvent &>( rhs ).TimingTrigger)
   , SpillStart(const_cast<ProtoDUNEBeamEvent &>( rhs ).SpillStart)
   , SpillOffset(const_cast<ProtoDUNEBeamEvent &>( rhs ).SpillOffset)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   ProtoDUNEBeamEvent &modrhs = const_cast<ProtoDUNEBeamEvent &>( rhs );
   modrhs.fiberMonitors.clear();
   modrhs.Tracks.clear();
   modrhs.RecoBeamMomenta.clear();
}
beam::ProtoDUNEBeamEvent::~ProtoDUNEBeamEvent() {
}
#endif // beam__ProtoDUNEBeamEvent_cxx

#ifndef beam__CKov_cxx
#define beam__CKov_cxx
beam::CKov::CKov() {
}
beam::CKov::CKov(const CKov & rhs)
   : trigger(const_cast<CKov &>( rhs ).trigger)
   , pressure(const_cast<CKov &>( rhs ).pressure)
   , timeStamp(const_cast<CKov &>( rhs ).timeStamp)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
beam::CKov::~CKov() {
}
#endif // beam__CKov_cxx

#ifndef art__Wrapper_vector_dune__ProtoDUNETimeStamp____cxx
#define art__Wrapper_vector_dune__ProtoDUNETimeStamp____cxx
art::Wrapper<vector<dune::ProtoDUNETimeStamp> >::Wrapper() {
}
art::Wrapper<vector<dune::ProtoDUNETimeStamp> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Wrapper &modrhs = const_cast<Wrapper &>( rhs );
   modrhs.obj.clear();
}
art::Wrapper<vector<dune::ProtoDUNETimeStamp> >::~Wrapper() {
}
#endif // art__Wrapper_vector_dune__ProtoDUNETimeStamp____cxx

#ifndef dune__ProtoDUNETimeStamp_cxx
#define dune__ProtoDUNETimeStamp_cxx
dune::ProtoDUNETimeStamp::ProtoDUNETimeStamp() {
}
dune::ProtoDUNETimeStamp::ProtoDUNETimeStamp(const ProtoDUNETimeStamp & rhs)
   : m_cookie(const_cast<ProtoDUNETimeStamp &>( rhs ).m_cookie)
   , m_triggerType(const_cast<ProtoDUNETimeStamp &>( rhs ).m_triggerType)
   , m_reservedBits(const_cast<ProtoDUNETimeStamp &>( rhs ).m_reservedBits)
   , m_timeStamp(const_cast<ProtoDUNETimeStamp &>( rhs ).m_timeStamp)
   , m_eventCounter(const_cast<ProtoDUNETimeStamp &>( rhs ).m_eventCounter)
   , m_checksumGood(const_cast<ProtoDUNETimeStamp &>( rhs ).m_checksumGood)
   , m_lastRunStart(const_cast<ProtoDUNETimeStamp &>( rhs ).m_lastRunStart)
   , m_lastSpillStart(const_cast<ProtoDUNETimeStamp &>( rhs ).m_lastSpillStart)
   , m_lastSpillEnd(const_cast<ProtoDUNETimeStamp &>( rhs ).m_lastSpillEnd)
   , m_version(const_cast<ProtoDUNETimeStamp &>( rhs ).m_version)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
dune::ProtoDUNETimeStamp::~ProtoDUNETimeStamp() {
}
#endif // dune__ProtoDUNETimeStamp_cxx

#ifndef art__Wrapper_vector_larpandoraobj__PFParticleMetadata____cxx
#define art__Wrapper_vector_larpandoraobj__PFParticleMetadata____cxx
art::Wrapper<vector<larpandoraobj::PFParticleMetadata> >::Wrapper() {
}
art::Wrapper<vector<larpandoraobj::PFParticleMetadata> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Wrapper &modrhs = const_cast<Wrapper &>( rhs );
   modrhs.obj.clear();
}
art::Wrapper<vector<larpandoraobj::PFParticleMetadata> >::~Wrapper() {
}
#endif // art__Wrapper_vector_larpandoraobj__PFParticleMetadata____cxx

#ifndef larpandoraobj__PFParticleMetadata_cxx
#define larpandoraobj__PFParticleMetadata_cxx
larpandoraobj::PFParticleMetadata::PFParticleMetadata() {
}
larpandoraobj::PFParticleMetadata::PFParticleMetadata(const PFParticleMetadata & rhs)
   : m_propertiesMap(const_cast<PFParticleMetadata &>( rhs ).m_propertiesMap)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   PFParticleMetadata &modrhs = const_cast<PFParticleMetadata &>( rhs );
   modrhs.m_propertiesMap.clear();
}
larpandoraobj::PFParticleMetadata::~PFParticleMetadata() {
}
#endif // larpandoraobj__PFParticleMetadata_cxx

#ifndef art__Wrapper_vector_raw__OpDetWaveform____cxx
#define art__Wrapper_vector_raw__OpDetWaveform____cxx
art::Wrapper<vector<raw::OpDetWaveform> >::Wrapper() {
}
art::Wrapper<vector<raw::OpDetWaveform> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Wrapper &modrhs = const_cast<Wrapper &>( rhs );
   modrhs.obj.clear();
}
art::Wrapper<vector<raw::OpDetWaveform> >::~Wrapper() {
}
#endif // art__Wrapper_vector_raw__OpDetWaveform____cxx

#ifndef raw__OpDetWaveform_cxx
#define raw__OpDetWaveform_cxx
raw::OpDetWaveform::OpDetWaveform() {
}
raw::OpDetWaveform::OpDetWaveform(const OpDetWaveform & rhs)
   : vector<short>(const_cast<OpDetWaveform &>( rhs ))
   , fChannel(const_cast<OpDetWaveform &>( rhs ).fChannel)
   , fTimeStamp(const_cast<OpDetWaveform &>( rhs ).fTimeStamp)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   OpDetWaveform &modrhs = const_cast<OpDetWaveform &>( rhs );
   modrhs.clear();
}
raw::OpDetWaveform::~OpDetWaveform() {
}
#endif // raw__OpDetWaveform_cxx

#ifndef art__Wrapper_vector_raw__RDStatus____cxx
#define art__Wrapper_vector_raw__RDStatus____cxx
art::Wrapper<vector<raw::RDStatus> >::Wrapper() {
}
art::Wrapper<vector<raw::RDStatus> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Wrapper &modrhs = const_cast<Wrapper &>( rhs );
   modrhs.obj.clear();
}
art::Wrapper<vector<raw::RDStatus> >::~Wrapper() {
}
#endif // art__Wrapper_vector_raw__RDStatus____cxx

#ifndef raw__RDStatus_cxx
#define raw__RDStatus_cxx
raw::RDStatus::RDStatus() {
}
raw::RDStatus::RDStatus(const RDStatus & rhs)
   : fCorruptDataDropped(const_cast<RDStatus &>( rhs ).fCorruptDataDropped)
   , fCorruptDataKept(const_cast<RDStatus &>( rhs ).fCorruptDataKept)
   , fStatWord(const_cast<RDStatus &>( rhs ).fStatWord)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
raw::RDStatus::~RDStatus() {
}
#endif // raw__RDStatus_cxx

#ifndef art__Wrapper_vector_raw__RDTimeStamp____cxx
#define art__Wrapper_vector_raw__RDTimeStamp____cxx
art::Wrapper<vector<raw::RDTimeStamp> >::Wrapper() {
}
art::Wrapper<vector<raw::RDTimeStamp> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Wrapper &modrhs = const_cast<Wrapper &>( rhs );
   modrhs.obj.clear();
}
art::Wrapper<vector<raw::RDTimeStamp> >::~Wrapper() {
}
#endif // art__Wrapper_vector_raw__RDTimeStamp____cxx

#ifndef raw__RDTimeStamp_cxx
#define raw__RDTimeStamp_cxx
raw::RDTimeStamp::RDTimeStamp() {
}
raw::RDTimeStamp::RDTimeStamp(const RDTimeStamp & rhs)
   : fTimeStamp(const_cast<RDTimeStamp &>( rhs ).fTimeStamp)
   , fFlags(const_cast<RDTimeStamp &>( rhs ).fFlags)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
raw::RDTimeStamp::~RDTimeStamp() {
}
#endif // raw__RDTimeStamp_cxx

#ifndef art__Wrapper_vector_raw__RawDigit____cxx
#define art__Wrapper_vector_raw__RawDigit____cxx
art::Wrapper<vector<raw::RawDigit> >::Wrapper() {
}
art::Wrapper<vector<raw::RawDigit> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Wrapper &modrhs = const_cast<Wrapper &>( rhs );
   modrhs.obj.clear();
}
art::Wrapper<vector<raw::RawDigit> >::~Wrapper() {
}
#endif // art__Wrapper_vector_raw__RawDigit____cxx

#ifndef raw__RawDigit_cxx
#define raw__RawDigit_cxx
raw::RawDigit::RawDigit() {
}
raw::RawDigit::RawDigit(const RawDigit & rhs)
   : fADC(const_cast<RawDigit &>( rhs ).fADC)
   , fChannel(const_cast<RawDigit &>( rhs ).fChannel)
   , fSamples(const_cast<RawDigit &>( rhs ).fSamples)
   , fPedestal(const_cast<RawDigit &>( rhs ).fPedestal)
   , fSigma(const_cast<RawDigit &>( rhs ).fSigma)
   , fCompression(const_cast<RawDigit &>( rhs ).fCompression)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   RawDigit &modrhs = const_cast<RawDigit &>( rhs );
   modrhs.fADC.clear();
}
raw::RawDigit::~RawDigit() {
}
#endif // raw__RawDigit_cxx

#ifndef art__Wrapper_vector_raw__ctb__pdspctb____cxx
#define art__Wrapper_vector_raw__ctb__pdspctb____cxx
art::Wrapper<vector<raw::ctb::pdspctb> >::Wrapper() {
}
art::Wrapper<vector<raw::ctb::pdspctb> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Wrapper &modrhs = const_cast<Wrapper &>( rhs );
   modrhs.obj.clear();
}
art::Wrapper<vector<raw::ctb::pdspctb> >::~Wrapper() {
}
#endif // art__Wrapper_vector_raw__ctb__pdspctb____cxx

#ifndef raw__ctb__pdspctb_cxx
#define raw__ctb__pdspctb_cxx
raw::ctb::pdspctb::pdspctb() {
}
raw::ctb::pdspctb::pdspctb(const pdspctb & rhs)
   : fTriggers(const_cast<pdspctb &>( rhs ).fTriggers)
   , fChStatuses(const_cast<pdspctb &>( rhs ).fChStatuses)
   , fFeedbacks(const_cast<pdspctb &>( rhs ).fFeedbacks)
   , fMiscs(const_cast<pdspctb &>( rhs ).fMiscs)
   , fIndexes(const_cast<pdspctb &>( rhs ).fIndexes)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   pdspctb &modrhs = const_cast<pdspctb &>( rhs );
   modrhs.fTriggers.clear();
   modrhs.fChStatuses.clear();
   modrhs.fFeedbacks.clear();
   modrhs.fMiscs.clear();
   modrhs.fIndexes.clear();
}
raw::ctb::pdspctb::~pdspctb() {
}
#endif // raw__ctb__pdspctb_cxx

#ifndef art__Wrapper_vector_recob__Cluster____cxx
#define art__Wrapper_vector_recob__Cluster____cxx
art::Wrapper<vector<recob::Cluster> >::Wrapper() {
}
art::Wrapper<vector<recob::Cluster> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Wrapper &modrhs = const_cast<Wrapper &>( rhs );
   modrhs.obj.clear();
}
art::Wrapper<vector<recob::Cluster> >::~Wrapper() {
}
#endif // art__Wrapper_vector_recob__Cluster____cxx

#ifndef recob__Cluster_cxx
#define recob__Cluster_cxx
recob::Cluster::Cluster() {
}
recob::Cluster::Cluster(const Cluster & rhs)
   : fNHits(const_cast<Cluster &>( rhs ).fNHits)
   , fMultipleHitDensity(const_cast<Cluster &>( rhs ).fMultipleHitDensity)
   , fWidth(const_cast<Cluster &>( rhs ).fWidth)
   , fID(const_cast<Cluster &>( rhs ).fID)
   , fView(const_cast<Cluster &>( rhs ).fView)
   , fPlaneID(const_cast<Cluster &>( rhs ).fPlaneID)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   for (Int_t i=0;i<2;i++) fEndWires[i] = rhs.fEndWires[i];
   for (Int_t i=0;i<2;i++) fSigmaEndWires[i] = rhs.fSigmaEndWires[i];
   for (Int_t i=0;i<2;i++) fEndTicks[i] = rhs.fEndTicks[i];
   for (Int_t i=0;i<2;i++) fSigmaEndTicks[i] = rhs.fSigmaEndTicks[i];
   for (Int_t i=0;i<2;i++) fEndCharges[i] = rhs.fEndCharges[i];
   for (Int_t i=0;i<2;i++) fAngles[i] = rhs.fAngles[i];
   for (Int_t i=0;i<2;i++) fOpeningAngles[i] = rhs.fOpeningAngles[i];
   for (Int_t i=0;i<2;i++) fChargeSum[i] = rhs.fChargeSum[i];
   for (Int_t i=0;i<2;i++) fChargeStdDev[i] = rhs.fChargeStdDev[i];
   for (Int_t i=0;i<2;i++) fChargeAverage[i] = rhs.fChargeAverage[i];
}
recob::Cluster::~Cluster() {
}
#endif // recob__Cluster_cxx

#ifndef art__Wrapper_vector_recob__EndPoint2D____cxx
#define art__Wrapper_vector_recob__EndPoint2D____cxx
art::Wrapper<vector<recob::EndPoint2D> >::Wrapper() {
}
art::Wrapper<vector<recob::EndPoint2D> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Wrapper &modrhs = const_cast<Wrapper &>( rhs );
   modrhs.obj.clear();
}
art::Wrapper<vector<recob::EndPoint2D> >::~Wrapper() {
}
#endif // art__Wrapper_vector_recob__EndPoint2D____cxx

#ifndef recob__EndPoint2D_cxx
#define recob__EndPoint2D_cxx
recob::EndPoint2D::EndPoint2D() {
}
recob::EndPoint2D::EndPoint2D(const EndPoint2D & rhs)
   : fDriftTime(const_cast<EndPoint2D &>( rhs ).fDriftTime)
   , fWireID(const_cast<EndPoint2D &>( rhs ).fWireID)
   , fID(const_cast<EndPoint2D &>( rhs ).fID)
   , fStrength(const_cast<EndPoint2D &>( rhs ).fStrength)
   , fView(const_cast<EndPoint2D &>( rhs ).fView)
   , fTotalCharge(const_cast<EndPoint2D &>( rhs ).fTotalCharge)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
recob::EndPoint2D::~EndPoint2D() {
}
#endif // recob__EndPoint2D_cxx

#ifndef geo__WireID_cxx
#define geo__WireID_cxx
geo::WireID::WireID() {
}
geo::WireID::WireID(const WireID & rhs)
   : geo::PlaneID(const_cast<WireID &>( rhs ))
   , Wire(const_cast<WireID &>( rhs ).Wire)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
geo::WireID::~WireID() {
}
#endif // geo__WireID_cxx

#ifndef art__Wrapper_vector_recob__Hit____cxx
#define art__Wrapper_vector_recob__Hit____cxx
art::Wrapper<vector<recob::Hit> >::Wrapper() {
}
art::Wrapper<vector<recob::Hit> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Wrapper &modrhs = const_cast<Wrapper &>( rhs );
   modrhs.obj.clear();
}
art::Wrapper<vector<recob::Hit> >::~Wrapper() {
}
#endif // art__Wrapper_vector_recob__Hit____cxx

#ifndef recob__Hit_cxx
#define recob__Hit_cxx
recob::Hit::Hit() {
}
recob::Hit::Hit(const Hit & rhs)
   : fChannel(const_cast<Hit &>( rhs ).fChannel)
   , fStartTick(const_cast<Hit &>( rhs ).fStartTick)
   , fEndTick(const_cast<Hit &>( rhs ).fEndTick)
   , fPeakTime(const_cast<Hit &>( rhs ).fPeakTime)
   , fSigmaPeakTime(const_cast<Hit &>( rhs ).fSigmaPeakTime)
   , fRMS(const_cast<Hit &>( rhs ).fRMS)
   , fPeakAmplitude(const_cast<Hit &>( rhs ).fPeakAmplitude)
   , fSigmaPeakAmplitude(const_cast<Hit &>( rhs ).fSigmaPeakAmplitude)
   , fSummedADC(const_cast<Hit &>( rhs ).fSummedADC)
   , fIntegral(const_cast<Hit &>( rhs ).fIntegral)
   , fSigmaIntegral(const_cast<Hit &>( rhs ).fSigmaIntegral)
   , fMultiplicity(const_cast<Hit &>( rhs ).fMultiplicity)
   , fLocalIndex(const_cast<Hit &>( rhs ).fLocalIndex)
   , fGoodnessOfFit(const_cast<Hit &>( rhs ).fGoodnessOfFit)
   , fNDF(const_cast<Hit &>( rhs ).fNDF)
   , fView(const_cast<Hit &>( rhs ).fView)
   , fSignalType(const_cast<Hit &>( rhs ).fSignalType)
   , fWireID(const_cast<Hit &>( rhs ).fWireID)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
recob::Hit::~Hit() {
}
#endif // recob__Hit_cxx

#ifndef art__Wrapper_vector_recob__OpFlash____cxx
#define art__Wrapper_vector_recob__OpFlash____cxx
art::Wrapper<vector<recob::OpFlash> >::Wrapper() {
}
art::Wrapper<vector<recob::OpFlash> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Wrapper &modrhs = const_cast<Wrapper &>( rhs );
   modrhs.obj.clear();
}
art::Wrapper<vector<recob::OpFlash> >::~Wrapper() {
}
#endif // art__Wrapper_vector_recob__OpFlash____cxx

#ifndef recob__OpFlash_cxx
#define recob__OpFlash_cxx
recob::OpFlash::OpFlash() {
}
recob::OpFlash::OpFlash(const OpFlash & rhs)
   : fTime(const_cast<OpFlash &>( rhs ).fTime)
   , fTimeWidth(const_cast<OpFlash &>( rhs ).fTimeWidth)
   , fAbsTime(const_cast<OpFlash &>( rhs ).fAbsTime)
   , fFrame(const_cast<OpFlash &>( rhs ).fFrame)
   , fPEperOpDet(const_cast<OpFlash &>( rhs ).fPEperOpDet)
   , fWireCenters(const_cast<OpFlash &>( rhs ).fWireCenters)
   , fWireWidths(const_cast<OpFlash &>( rhs ).fWireWidths)
   , fYCenter(const_cast<OpFlash &>( rhs ).fYCenter)
   , fYWidth(const_cast<OpFlash &>( rhs ).fYWidth)
   , fZCenter(const_cast<OpFlash &>( rhs ).fZCenter)
   , fZWidth(const_cast<OpFlash &>( rhs ).fZWidth)
   , fFastToTotal(const_cast<OpFlash &>( rhs ).fFastToTotal)
   , fInBeamFrame(const_cast<OpFlash &>( rhs ).fInBeamFrame)
   , fOnBeamTime(const_cast<OpFlash &>( rhs ).fOnBeamTime)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   OpFlash &modrhs = const_cast<OpFlash &>( rhs );
   modrhs.fPEperOpDet.clear();
   modrhs.fWireCenters.clear();
   modrhs.fWireWidths.clear();
}
recob::OpFlash::~OpFlash() {
}
#endif // recob__OpFlash_cxx

#ifndef art__Wrapper_vector_recob__OpHit____cxx
#define art__Wrapper_vector_recob__OpHit____cxx
art::Wrapper<vector<recob::OpHit> >::Wrapper() {
}
art::Wrapper<vector<recob::OpHit> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Wrapper &modrhs = const_cast<Wrapper &>( rhs );
   modrhs.obj.clear();
}
art::Wrapper<vector<recob::OpHit> >::~Wrapper() {
}
#endif // art__Wrapper_vector_recob__OpHit____cxx

#ifndef recob__OpHit_cxx
#define recob__OpHit_cxx
recob::OpHit::OpHit() {
}
recob::OpHit::OpHit(const OpHit & rhs)
   : fOpChannel(const_cast<OpHit &>( rhs ).fOpChannel)
   , fFrame(const_cast<OpHit &>( rhs ).fFrame)
   , fPeakTime(const_cast<OpHit &>( rhs ).fPeakTime)
   , fPeakTimeAbs(const_cast<OpHit &>( rhs ).fPeakTimeAbs)
   , fWidth(const_cast<OpHit &>( rhs ).fWidth)
   , fArea(const_cast<OpHit &>( rhs ).fArea)
   , fAmplitude(const_cast<OpHit &>( rhs ).fAmplitude)
   , fPE(const_cast<OpHit &>( rhs ).fPE)
   , fFastToTotal(const_cast<OpHit &>( rhs ).fFastToTotal)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
recob::OpHit::~OpHit() {
}
#endif // recob__OpHit_cxx

#ifndef art__Wrapper_vector_recob__PCAxis____cxx
#define art__Wrapper_vector_recob__PCAxis____cxx
art::Wrapper<vector<recob::PCAxis> >::Wrapper() {
}
art::Wrapper<vector<recob::PCAxis> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Wrapper &modrhs = const_cast<Wrapper &>( rhs );
   modrhs.obj.clear();
}
art::Wrapper<vector<recob::PCAxis> >::~Wrapper() {
}
#endif // art__Wrapper_vector_recob__PCAxis____cxx

#ifndef recob__PCAxis_cxx
#define recob__PCAxis_cxx
recob::PCAxis::PCAxis() {
}
recob::PCAxis::PCAxis(const PCAxis & rhs)
   : fSvdOK(const_cast<PCAxis &>( rhs ).fSvdOK)
   , fNumHitsUsed(const_cast<PCAxis &>( rhs ).fNumHitsUsed)
   , fEigenVectors(const_cast<PCAxis &>( rhs ).fEigenVectors)
   , fAveHitDoca(const_cast<PCAxis &>( rhs ).fAveHitDoca)
   , fID(const_cast<PCAxis &>( rhs ).fID)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   for (Int_t i=0;i<3;i++) fEigenValues[i] = rhs.fEigenValues[i];
   PCAxis &modrhs = const_cast<PCAxis &>( rhs );
   modrhs.fEigenVectors.clear();
   for (Int_t i=0;i<3;i++) fAvePosition[i] = rhs.fAvePosition[i];
}
recob::PCAxis::~PCAxis() {
}
#endif // recob__PCAxis_cxx

#ifndef art__Wrapper_vector_recob__PFParticle____cxx
#define art__Wrapper_vector_recob__PFParticle____cxx
art::Wrapper<vector<recob::PFParticle> >::Wrapper() {
}
art::Wrapper<vector<recob::PFParticle> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Wrapper &modrhs = const_cast<Wrapper &>( rhs );
   modrhs.obj.clear();
}
art::Wrapper<vector<recob::PFParticle> >::~Wrapper() {
}
#endif // art__Wrapper_vector_recob__PFParticle____cxx

#ifndef recob__PFParticle_cxx
#define recob__PFParticle_cxx
recob::PFParticle::PFParticle() {
}
recob::PFParticle::PFParticle(const PFParticle & rhs)
   : fPdgCode(const_cast<PFParticle &>( rhs ).fPdgCode)
   , fSelf(const_cast<PFParticle &>( rhs ).fSelf)
   , fParent(const_cast<PFParticle &>( rhs ).fParent)
   , fDaughters(const_cast<PFParticle &>( rhs ).fDaughters)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   PFParticle &modrhs = const_cast<PFParticle &>( rhs );
   modrhs.fDaughters.clear();
}
recob::PFParticle::~PFParticle() {
}
#endif // recob__PFParticle_cxx

#ifndef art__Wrapper_vector_recob__PointCharge____cxx
#define art__Wrapper_vector_recob__PointCharge____cxx
art::Wrapper<vector<recob::PointCharge> >::Wrapper() {
}
art::Wrapper<vector<recob::PointCharge> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Wrapper &modrhs = const_cast<Wrapper &>( rhs );
   modrhs.obj.clear();
}
art::Wrapper<vector<recob::PointCharge> >::~Wrapper() {
}
#endif // art__Wrapper_vector_recob__PointCharge____cxx

#ifndef recob__PointCharge_cxx
#define recob__PointCharge_cxx
recob::PointCharge::PointCharge() {
}
recob::PointCharge::PointCharge(const PointCharge & rhs)
   : fCharge(const_cast<PointCharge &>( rhs ).fCharge)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
recob::PointCharge::~PointCharge() {
}
#endif // recob__PointCharge_cxx

#ifndef art__Wrapper_vector_recob__Shower____cxx
#define art__Wrapper_vector_recob__Shower____cxx
art::Wrapper<vector<recob::Shower> >::Wrapper() {
}
art::Wrapper<vector<recob::Shower> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Wrapper &modrhs = const_cast<Wrapper &>( rhs );
   modrhs.obj.clear();
}
art::Wrapper<vector<recob::Shower> >::~Wrapper() {
}
#endif // art__Wrapper_vector_recob__Shower____cxx

#ifndef recob__Shower_cxx
#define recob__Shower_cxx
recob::Shower::Shower() {
}
recob::Shower::Shower(const Shower & rhs)
   : fID(const_cast<Shower &>( rhs ).fID)
   , fDCosStart(const_cast<Shower &>( rhs ).fDCosStart)
   , fSigmaDCosStart(const_cast<Shower &>( rhs ).fSigmaDCosStart)
   , fXYZstart(const_cast<Shower &>( rhs ).fXYZstart)
   , fSigmaXYZstart(const_cast<Shower &>( rhs ).fSigmaXYZstart)
   , fTotalEnergy(const_cast<Shower &>( rhs ).fTotalEnergy)
   , fSigmaTotalEnergy(const_cast<Shower &>( rhs ).fSigmaTotalEnergy)
   , fdEdx(const_cast<Shower &>( rhs ).fdEdx)
   , fSigmadEdx(const_cast<Shower &>( rhs ).fSigmadEdx)
   , fTotalMIPEnergy(const_cast<Shower &>( rhs ).fTotalMIPEnergy)
   , fSigmaTotalMIPEnergy(const_cast<Shower &>( rhs ).fSigmaTotalMIPEnergy)
   , fBestPlane(const_cast<Shower &>( rhs ).fBestPlane)
   , fLength(const_cast<Shower &>( rhs ).fLength)
   , fOpenAngle(const_cast<Shower &>( rhs ).fOpenAngle)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Shower &modrhs = const_cast<Shower &>( rhs );
   modrhs.fTotalEnergy.clear();
   modrhs.fSigmaTotalEnergy.clear();
   modrhs.fdEdx.clear();
   modrhs.fSigmadEdx.clear();
   modrhs.fTotalMIPEnergy.clear();
   modrhs.fSigmaTotalMIPEnergy.clear();
}
recob::Shower::~Shower() {
}
#endif // recob__Shower_cxx

#ifndef art__Wrapper_vector_recob__SpacePoint____cxx
#define art__Wrapper_vector_recob__SpacePoint____cxx
art::Wrapper<vector<recob::SpacePoint> >::Wrapper() {
}
art::Wrapper<vector<recob::SpacePoint> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Wrapper &modrhs = const_cast<Wrapper &>( rhs );
   modrhs.obj.clear();
}
art::Wrapper<vector<recob::SpacePoint> >::~Wrapper() {
}
#endif // art__Wrapper_vector_recob__SpacePoint____cxx

#ifndef recob__SpacePoint_cxx
#define recob__SpacePoint_cxx
recob::SpacePoint::SpacePoint() {
}
recob::SpacePoint::SpacePoint(const SpacePoint & rhs)
   : fID(const_cast<SpacePoint &>( rhs ).fID)
   , fChisq(const_cast<SpacePoint &>( rhs ).fChisq)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   for (Int_t i=0;i<3;i++) fXYZ[i] = rhs.fXYZ[i];
   for (Int_t i=0;i<6;i++) fErrXYZ[i] = rhs.fErrXYZ[i];
}
recob::SpacePoint::~SpacePoint() {
}
#endif // recob__SpacePoint_cxx

#ifndef art__Wrapper_vector_recob__Track____cxx
#define art__Wrapper_vector_recob__Track____cxx
art::Wrapper<vector<recob::Track> >::Wrapper() {
}
art::Wrapper<vector<recob::Track> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Wrapper &modrhs = const_cast<Wrapper &>( rhs );
   modrhs.obj.clear();
}
art::Wrapper<vector<recob::Track> >::~Wrapper() {
}
#endif // art__Wrapper_vector_recob__Track____cxx

#ifndef recob__Track_cxx
#define recob__Track_cxx
recob::Track::Track() {
}
recob::Track::Track(const Track & rhs)
   : fTraj(const_cast<Track &>( rhs ).fTraj)
   , fPId(const_cast<Track &>( rhs ).fPId)
   , fChi2(const_cast<Track &>( rhs ).fChi2)
   , fNdof(const_cast<Track &>( rhs ).fNdof)
   , fCovVertex(const_cast<Track &>( rhs ).fCovVertex)
   , fCovEnd(const_cast<Track &>( rhs ).fCovEnd)
   , fID(const_cast<Track &>( rhs ).fID)
   , fdQdx(const_cast<Track &>( rhs ).fdQdx)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Track &modrhs = const_cast<Track &>( rhs );
   modrhs.fdQdx.clear();
}
recob::Track::~Track() {
}
#endif // recob__Track_cxx

#ifndef recob__TrackTrajectory_cxx
#define recob__TrackTrajectory_cxx
recob::TrackTrajectory::TrackTrajectory() {
}
recob::TrackTrajectory::TrackTrajectory(const TrackTrajectory & rhs)
   : recob::Trajectory(const_cast<TrackTrajectory &>( rhs ))
   , fFlags(const_cast<TrackTrajectory &>( rhs ).fFlags)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   TrackTrajectory &modrhs = const_cast<TrackTrajectory &>( rhs );
   modrhs.fFlags.clear();
}
recob::TrackTrajectory::~TrackTrajectory() {
}
#endif // recob__TrackTrajectory_cxx

#ifndef recob__Trajectory_cxx
#define recob__Trajectory_cxx
recob::Trajectory::Trajectory() {
}
recob::Trajectory::Trajectory(const Trajectory & rhs)
   : fPositions(const_cast<Trajectory &>( rhs ).fPositions)
   , fMomenta(const_cast<Trajectory &>( rhs ).fMomenta)
   , fHasMomentum(const_cast<Trajectory &>( rhs ).fHasMomentum)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Trajectory &modrhs = const_cast<Trajectory &>( rhs );
   modrhs.fPositions.clear();
   modrhs.fMomenta.clear();
}
recob::Trajectory::~Trajectory() {
}
#endif // recob__Trajectory_cxx

#ifndef art__Wrapper_vector_recob__Vertex____cxx
#define art__Wrapper_vector_recob__Vertex____cxx
art::Wrapper<vector<recob::Vertex> >::Wrapper() {
}
art::Wrapper<vector<recob::Vertex> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Wrapper &modrhs = const_cast<Wrapper &>( rhs );
   modrhs.obj.clear();
}
art::Wrapper<vector<recob::Vertex> >::~Wrapper() {
}
#endif // art__Wrapper_vector_recob__Vertex____cxx

#ifndef recob__Vertex_cxx
#define recob__Vertex_cxx
recob::Vertex::Vertex() {
}
recob::Vertex::Vertex(const Vertex & rhs)
   : pos_(const_cast<Vertex &>( rhs ).pos_)
   , cov_(const_cast<Vertex &>( rhs ).cov_)
   , chi2_(const_cast<Vertex &>( rhs ).chi2_)
   , ndof_(const_cast<Vertex &>( rhs ).ndof_)
   , status_(const_cast<Vertex &>( rhs ).status_)
   , id_(const_cast<Vertex &>( rhs ).id_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
recob::Vertex::~Vertex() {
}
#endif // recob__Vertex_cxx

#ifndef art__Wrapper_vector_recob__Wire____cxx
#define art__Wrapper_vector_recob__Wire____cxx
art::Wrapper<vector<recob::Wire> >::Wrapper() {
}
art::Wrapper<vector<recob::Wire> >::Wrapper(const Wrapper & rhs)
   : art::EDProduct(const_cast<Wrapper &>( rhs ))
   , present(const_cast<Wrapper &>( rhs ).present)
   , rangeSetID(const_cast<Wrapper &>( rhs ).rangeSetID)
   , obj(const_cast<Wrapper &>( rhs ).obj)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   Wrapper &modrhs = const_cast<Wrapper &>( rhs );
   modrhs.obj.clear();
}
art::Wrapper<vector<recob::Wire> >::~Wrapper() {
}
#endif // art__Wrapper_vector_recob__Wire____cxx

#ifndef recob__Wire_cxx
#define recob__Wire_cxx
recob::Wire::Wire() {
}
recob::Wire::Wire(const Wire & rhs)
   : fChannel(const_cast<Wire &>( rhs ).fChannel)
   , fView(const_cast<Wire &>( rhs ).fView)
   , fSignalROI(const_cast<Wire &>( rhs ).fSignalROI)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
recob::Wire::~Wire() {
}
#endif // recob__Wire_cxx

#ifndef lar__sparse_vector_float___datarange_t_cxx
#define lar__sparse_vector_float___datarange_t_cxx
lar::sparse_vector<float>::datarange_t::datarange_t() {
}
lar::sparse_vector<float>::datarange_t::datarange_t(const datarange_t & rhs)
   : lar::range_t<unsigned long>(const_cast<datarange_t &>( rhs ))
   , values(const_cast<datarange_t &>( rhs ).values)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   datarange_t &modrhs = const_cast<datarange_t &>( rhs );
   modrhs.values.clear();
}
lar::sparse_vector<float>::datarange_t::~datarange_t() {
}
#endif // lar__sparse_vector_float___datarange_t_cxx

#ifndef lar__sparse_vector_float__cxx
#define lar__sparse_vector_float__cxx
lar::sparse_vector<float>::sparse_vector() {
}
lar::sparse_vector<float>::sparse_vector(const sparse_vector & rhs)
   : nominal_size(const_cast<sparse_vector &>( rhs ).nominal_size)
   , ranges(const_cast<sparse_vector &>( rhs ).ranges)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   sparse_vector &modrhs = const_cast<sparse_vector &>( rhs );
   modrhs.ranges.clear();
}
lar::sparse_vector<float>::~sparse_vector() {
}
#endif // lar__sparse_vector_float__cxx

#ifndef art__RefCore__RefCoreTransients_cxx
#define art__RefCore__RefCoreTransients_cxx
art::RefCore::RefCoreTransients::RefCoreTransients() {
}
art::RefCore::RefCoreTransients::RefCoreTransients(const RefCoreTransients & rhs)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
art::RefCore::RefCoreTransients::~RefCoreTransients() {
}
#endif // art__RefCore__RefCoreTransients_cxx

#ifndef art__RefCore_cxx
#define art__RefCore_cxx
art::RefCore::RefCore() {
}
art::RefCore::RefCore(const RefCore & rhs)
   : id_(const_cast<RefCore &>( rhs ).id_)
   , transients_(const_cast<RefCore &>( rhs ).transients_)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
art::RefCore::~RefCore() {
}
#endif // art__RefCore_cxx

#ifndef recob__TrackHitMeta_cxx
#define recob__TrackHitMeta_cxx
recob::TrackHitMeta::TrackHitMeta() {
}
recob::TrackHitMeta::TrackHitMeta(const TrackHitMeta & rhs)
   : fIndex(const_cast<TrackHitMeta &>( rhs ).fIndex)
   , fDx(const_cast<TrackHitMeta &>( rhs ).fDx)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
recob::TrackHitMeta::~TrackHitMeta() {
}
#endif // recob__TrackHitMeta_cxx

#ifndef beam__FBM_cxx
#define beam__FBM_cxx
beam::FBM::FBM() {
}
beam::FBM::FBM(const FBM & rhs)
   : timeStamp(const_cast<FBM &>( rhs ).timeStamp)
   , ID(const_cast<FBM &>( rhs ).ID)
   , active(const_cast<FBM &>( rhs ).active)
   , decoded(const_cast<FBM &>( rhs ).decoded)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
   for (Int_t i=0;i<192;i++) fibers[i] = rhs.fibers[i];
   for (Int_t i=0;i<6;i++) fiberData[i] = rhs.fiberData[i];
   for (Int_t i=0;i<4;i++) timeData[i] = rhs.timeData[i];
   FBM &modrhs = const_cast<FBM &>( rhs );
   modrhs.active.clear();
}
beam::FBM::~FBM() {
}
#endif // beam__FBM_cxx

#ifndef raw__ctb__Trigger_cxx
#define raw__ctb__Trigger_cxx
raw::ctb::Trigger::Trigger() {
}
raw::ctb::Trigger::Trigger(const Trigger & rhs)
   : word_type(const_cast<Trigger &>( rhs ).word_type)
   , trigger_word(const_cast<Trigger &>( rhs ).trigger_word)
   , timestamp(const_cast<Trigger &>( rhs ).timestamp)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
raw::ctb::Trigger::~Trigger() {
}
#endif // raw__ctb__Trigger_cxx

#ifndef raw__ctb__ChStatus_cxx
#define raw__ctb__ChStatus_cxx
raw::ctb::ChStatus::ChStatus() {
}
raw::ctb::ChStatus::ChStatus(const ChStatus & rhs)
   : word_type(const_cast<ChStatus &>( rhs ).word_type)
   , pds(const_cast<ChStatus &>( rhs ).pds)
   , crt(const_cast<ChStatus &>( rhs ).crt)
   , beam_hi(const_cast<ChStatus &>( rhs ).beam_hi)
   , beam_lo(const_cast<ChStatus &>( rhs ).beam_lo)
   , timestamp(const_cast<ChStatus &>( rhs ).timestamp)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
raw::ctb::ChStatus::~ChStatus() {
}
#endif // raw__ctb__ChStatus_cxx

#ifndef raw__ctb__Misc_cxx
#define raw__ctb__Misc_cxx
raw::ctb::Misc::Misc() {
}
raw::ctb::Misc::Misc(const Misc & rhs)
   : word_type(const_cast<Misc &>( rhs ).word_type)
   , payload(const_cast<Misc &>( rhs ).payload)
   , timestamp(const_cast<Misc &>( rhs ).timestamp)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
raw::ctb::Misc::~Misc() {
}
#endif // raw__ctb__Misc_cxx

#ifndef raw__ctb__WordIndex_cxx
#define raw__ctb__WordIndex_cxx
raw::ctb::WordIndex::WordIndex() {
}
raw::ctb::WordIndex::WordIndex(const WordIndex & rhs)
   : word_type(const_cast<WordIndex &>( rhs ).word_type)
   , index(const_cast<WordIndex &>( rhs ).index)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
raw::ctb::WordIndex::~WordIndex() {
}
#endif // raw__ctb__WordIndex_cxx

#ifndef recob__TrajectoryPointFlags_cxx
#define recob__TrajectoryPointFlags_cxx
recob::TrajectoryPointFlags::TrajectoryPointFlags() {
}
recob::TrajectoryPointFlags::TrajectoryPointFlags(const TrajectoryPointFlags & rhs)
   : fFromHit(const_cast<TrajectoryPointFlags &>( rhs ).fFromHit)
   , fFlags(const_cast<TrajectoryPointFlags &>( rhs ).fFlags)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
recob::TrajectoryPointFlags::~TrajectoryPointFlags() {
}
#endif // recob__TrajectoryPointFlags_cxx

#ifndef util__flags__FlagSet_32_unsigned_int__cxx
#define util__flags__FlagSet_32_unsigned_int__cxx
util::flags::FlagSet<32,unsigned int>::FlagSet() {
}
util::flags::FlagSet<32,unsigned int>::FlagSet(const FlagSet & rhs)
   : util::flags::BitMask<unsigned int>(const_cast<FlagSet &>( rhs ))
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
util::flags::FlagSet<32,unsigned int>::~FlagSet() {
}
#endif // util__flags__FlagSet_32_unsigned_int__cxx

#ifndef util__flags__BitMask_unsigned_int__cxx
#define util__flags__BitMask_unsigned_int__cxx
util::flags::BitMask<unsigned int>::BitMask() {
}
util::flags::BitMask<unsigned int>::BitMask(const BitMask & rhs)
   : values(const_cast<BitMask &>( rhs ).values)
   , presence(const_cast<BitMask &>( rhs ).presence)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
util::flags::BitMask<unsigned int>::~BitMask() {
}
#endif // util__flags__BitMask_unsigned_int__cxx

#ifndef util__flags__Bits_t_unsigned_int__cxx
#define util__flags__Bits_t_unsigned_int__cxx
util::flags::Bits_t<unsigned int>::Bits_t() {
}
util::flags::Bits_t<unsigned int>::Bits_t(const Bits_t & rhs)
   : data(const_cast<Bits_t &>( rhs ).data)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
util::flags::Bits_t<unsigned int>::~Bits_t() {
}
#endif // util__flags__Bits_t_unsigned_int__cxx

#ifndef lar__range_t_unsigned_long__cxx
#define lar__range_t_unsigned_long__cxx
lar::range_t<unsigned long>::range_t() {
}
lar::range_t<unsigned long>::range_t(const range_t & rhs)
   : offset(const_cast<range_t &>( rhs ).offset)
   , last(const_cast<range_t &>( rhs ).last)
{
   // This is NOT a copy constructor. This is actually a move constructor (for stl container's sake).
   // Use at your own risk!
   (void)rhs; // avoid warning about unused parameter
}
lar::range_t<unsigned long>::~range_t() {
}
#endif // lar__range_t_unsigned_long__cxx

