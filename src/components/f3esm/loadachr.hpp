#pragma once

/*
Placed NPC

## Format

Count | Subrecord | Name | Type | Info
------|-------|------|------|-----
+ | EDID | Editor ID | cstring | Editor ID
+ | NAME | Base | formid | FormID of an [NPC_](NPC_.md) record.
- | XEZN | Encounter Zone | formid | FormID of an [ECZN](ECZN.md) record.
- | XRGD | Ragdoll Data | ?? | ??
- | XRGB | Ragdoll Biped Data | ?? | ??
+ | XPRD | Idle Time | float32 | Patrol data
+ | XPPA | Patrol Script Marker | null | Patrol data
+ | INAM | Idle | formid | Patrol data. FormID of an [IDLE](IDLE.md) record, or null.
+ | | [Embedded Script](Subrecords/Script.md) | collection | Patrol data.
+ | TNAM | Topic | formid | Patrol data. FormID of a [DIAL](DIAL.md) record, or null.
- | XLCM | Level Modifier | int32 |
- | XMRC | Merchant Container | formid | FormID of a [REFR](REFR.md) record.
- | XCNT | Count | int32 |
- | XRDS | Radius | float32 |
- | XHLP | Health | float32 |
-* | [XDCR](Subrecords/XDCR.md) | Decal | struct | Linked decals
- | XLKR | Linked Reference | formid | FormID of a [REFR](REFR.md), [ACRE](ACRE.md), [ACHR](ACHR.md), [PGRE](PGRE.md) or [PMIS](PMIS.md) record.
- | [XCLP](Subrecords/XCLP.md) | Linked Reference Color | struct |
- | [XAPD](Subrecords/XAPD.md) | Flags | uint8 | Activate parents.
-* | [XAPR](Subrecords/XAPR.md) | Activate Parent Ref | struct | Activate parents
- | [XESP](Subrecords/XESP.md) | Enable Parent | struct |
- | XEMI | Emittance | formid | FormID of a [LIGH](LIGH.md) or [REGN](REGN.md) record.
- | XMBR | MultiBound Reference | formid | FormID of a [REFR](REFR.md) record.
- | XIBS | Ignored By Sandbox | null | Flag
- | XSCL | Scale | float32 |
+ | [DATA](Subrecords/DATA (ACHR, ACRE).md) | Position / Rotation | struct |
*/
