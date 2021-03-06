// Copyright (c) 1995-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of the License "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// e32\memmodel\epoc\multiple\mglobals.cpp
// 
//

#include "memmodel.h"

DMemModelChunk* MM::SvStackChunk;
TInt MM::MaxPagesInOneGo;
TBitMapAllocator* MM::UserCodeAllocator;
TBitMapAllocator* MM::DllDataAllocator;
const SRamZone* MmuBase::RamZoneConfig=NULL;	// Initialise to NULL in case RAM defrag is not required
TRamZoneCallback MmuBase::RamZoneCallback=NULL; // Initialise to NULL in case RAM defrag is not required
