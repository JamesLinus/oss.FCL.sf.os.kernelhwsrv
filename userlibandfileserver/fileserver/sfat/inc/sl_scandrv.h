// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// f32\sfat\inc\sl_scandrv.h
// 
//

/**
 @file
 @internalTechnology
*/

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!
//!! WARNING!! DO NOT edit this file !! '\sfat' component is obsolete and is not being used. '\sfat32'replaces it
//!!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


#ifndef SL_SCANDRV_H
#define SL_SCANDRV_H

//---------------------------------------------------------------------------------------------------------------------------------

const TInt KMaxMatchingEntries      = 2;        ///< Maximum number of matching directory entries scan drive can fix. Any more indicates a fault in the file system
const TInt KMaxArrayDepth           = 6;        ///< Maximum array depth for cluster storage when KMaxScanDepth is reached

//---------------------------------------------------------------------------------------------------------------------------------

/**
Data structure used to store the location of a partial VFat entry
*/
struct TPartVFatEntry
    {
    TEntryPos    iEntryPos; ///< The position of the partial VFat entry
    TFatDirEntry iEntry;    ///< The Dos entry The VFat entries belong with
    };

//---------------------------------------------------------------------------------------------------------------------------------

/**
Data structure used to store the locations of entries with matching
start cluster numbers. 
*/
struct TMatchingStartCluster
    {
    TEntryPos   iEntries[KMaxMatchingEntries]; ///< The positions of the matching entries
    TInt        iCount;         ///< Count of matching entries
    TInt        iStartCluster;  ///< The matching cluster number found in more than one entry
    };


//---------------------------------------------------------------------------------------------------------------------------------

class CCheckFatTable : public CBase
    {
public:
    static CCheckFatTable* NewL(CFatMountCB* aOwner);
    CCheckFatTable(CFatMountCB* aOwner);
    ~CCheckFatTable();
    void InitializeL();
    TBool FlushL();
    TBool GetNextClusterL(TInt& aCluster) const;
    void WriteFatEntryEofFL(TInt aCluster);
    TInt ReadL(TInt aFatIndex) const;
    void WriteL(TInt aFatIndex,TInt aValue);
private:
    void WriteMediaDescriptor();
    TInt PosInBytes(TInt aFatIndex) const;
    TInt PosInIndex(TInt aBytePos) const;
    inline TBool IsEof16Bit(TInt aCluster) const;
    inline TBool IsEof12Bit(TInt aCluster) const;
    inline TInt MaxFatIndex() const;
protected:
    TInt iMaxFatIndex;
    TUint8* iCheckFat;
    CFatMountCB* iOwner;
    };


//---------------------------------------------------------------------------------------------------------------------------------

/**
Scan drive class performs scan drive functionality on all types
of fat volume.
*/
class CScanDrive : public CBase
    {
public:
    enum TDirError{EScanMatchingEntry=1,EScanPartEntry};
public:
    CScanDrive();
    ~CScanDrive();
    static CScanDrive* NewL(CFatMountCB* aMount);
    void ConstructL(CFatMountCB* aMount);
    TInt StartL();
    TBool ProblemsDiscovered() const;  


private:

#if defined(DEBUG_SCANDRIVE)
    void PrintErrors();
    void CompareFatsL() const;
#endif

    void FixupDirErrorL();
    void FindSameStartClusterL();
    TInt FindStartClusterL(TInt aDirCluster);
    void CheckDirStructureL();
    void CheckDirL(TInt aCluster);
    void ProcessEntryL(const TFatDirEntry& aEntry);
    TInt CheckEntryClusterL(const TFatDirEntry& aEntry, const TEntryPos& aEntryPos);
    void WriteClusterChainL(TInt aCluster,TInt aSizeInBytes);
    TBool MoveToVFatEndL(TEntryPos& aPos,TFatDirEntry& aEntry,TInt& aDirLength);
    TBool IsValidVFatEntry(const TFatDirEntry& aEntry,TInt prevNum)const;
    TBool IsDosEntry(const TFatDirEntry& aEntry)const;
    void AddPartialVFatL(const TEntryPos& aStartPos, const TFatDirEntry& aEntry);
    TBool AddMatchingEntryL(const TEntryPos& aEntryPos);
    TInt GetReservedidL(const TEntryPos aVFatPos);
    void WriteNewFatsL();
    void FixPartEntryL();
    void FixMatchingEntryL();
    void MovePastEntriesL(TEntryPos& aEntryPos,TFatDirEntry& aEntry,TInt aToMove,TInt& aDirEntries);
    void AddToClusterListL(TInt aCluster);
    inline TBool AlreadyExistsL(TInt aCluster)const;
    inline TBool IsEndOfRootDir(const TEntryPos& aPos)const;
    inline TBool IsEofF(TInt aVal)const;
    inline TBool IsDirError()const;
    void IndicateErrorsFound();

private:
    CFatMountCB*        iMount;
    CCheckFatTable*     iNewFat;
    TPartVFatEntry      iPartEntry;
    TMatchingStartCluster iMatching;
    TDirError           iDirError;
    TInt                iDirsChecked;
    TInt                iRecursiveDepth;
    RArray<TInt>*       iClusterListArray[KMaxArrayDepth];
    TInt                iListArrayIndex;
    TBool               iFoundProblems; ///< if ETrue after finish, it means that there where some problems FS structure and they were probably fixed;
    };





#endif //SL_SCANDRV_H


