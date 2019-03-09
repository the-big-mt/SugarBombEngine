#ifndef OPENMW_ESM_ACTI_H
#define OPENMW_ESM_ACTI_H

#include <string>

/*
Activator

## Format

Count | Subrecord | Name | Type | Info
------|-------|------|------|-----
+ | EDID | Editor ID | cstring | 
+ | [OBND](Subrecords/OBND.md) | Object Bounds | struct |
- | FULL | Name | cstring | 
- | | [Model Data](Subrecords/Model.md) | collection |
- | SCRI | Script | formid | FormID of a [SCPT](SCPT.md) record.
- | | [Destruction Data](Subrecords/Destruction.md) | collection |
- | SNAM | Sound - Looping | formid | FormID of a [SOUN](SOUN.md) record.
- | VNAM | Sound - Activation | formid | FormID of a [SOUN](SOUN.md) record.
- | RNAM | Radio Station | formid | FormID of a [TACT](TACT.md) record.
- | WNAM | Water Type | formid | FormID of a [WATR](WATR.md) record.
*/

namespace ESM
{

class ESMReader;
class ESMWriter;

struct Activator
{
    static unsigned int sRecordId;
    /// Return a string descriptor for this record type. Currently used for debugging / error logs only.
    static std::string getRecordType() { return "Activator"; }

    std::string mId; // Editor ID
	std::string mName; // Activator name
	std::string mScript;
	std::string mModel;

    void load(ESMReader &esm, bool &isDeleted);
    void save(ESMWriter &esm, bool isDeleted = false) const;

    void blank();
    ///< Set record to default state (does not touch the ID).
};

}
#endif
