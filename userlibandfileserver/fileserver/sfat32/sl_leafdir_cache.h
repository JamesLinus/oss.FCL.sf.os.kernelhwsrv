// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// f32\sfat32\inc\sl_leafdir_cache.h
// 
//

/**
 @file
 @internalTechnology
*/

#ifndef SL_LEAFDIR_CACHE_H
#define SL_LEAFDIR_CACHE_H

//---------------------------------------------------------------------------------------------------------------------------------

class CLeafDirTree;

/*
A data structure to define the location of a direcotry 
*/
class TLeafDirData
	{
public:
	TLeafDirData();
	TLeafDirData(TUint aClusterNum);
	TLeafDirData(TUint aClusterNum, const TEntryPos& aMRUPos);
	inline TBool operator==(const TLeafDirData &aDirPos) const;
	inline TBool operator!=(const TLeafDirData &aDirPos) const;
public:
	TUint 		iClusterNum;
	TEntryPos	iMRUPos;
	};

/*
Class definition for the nodes of the leaf dir cache
All cached direcotries are stored as nodes in a tree structure
 
*/
class CLeafDirTreeNode : public CBase
	{
public:
	// Types of the nodes
    enum  TLeafDirTreeNodeType
        {
        ERoot,				// Represents the root node, i.e. the root path
        ELeaf,				// Represents a pure 'leaf' nodes, no child
        ELeafIntermediary,	// Represents a 'leaf' node, but has at least one child
        EPureIntermediary,	// Represents a pure 'intermediary' node, which only exists to share the common path of other nodes
        };

    static CLeafDirTreeNode* NewL(CLeafDirTree* aOwnerTree, const TDesC& aPathName, const TLeafDirData& aDirPos, TLeafDirTreeNodeType aType);
	~CLeafDirTreeNode();
	inline CLeafDirTreeNode* Parent();
	inline void SetParent(CLeafDirTreeNode* aNode);
	inline RPointerArray<CLeafDirTreeNode>& Children();
	void MakeItChildL(CLeafDirTreeNode* aNode);
	TInt RemoveChild(CLeafDirTreeNode* aNode);
	inline const TLeafDirData& LeafDirData() const;
	inline TUint32 StartClusterNum() const;
	inline void SetLeafDirData(const TLeafDirData& aLeafDirData);
	inline const TDesC& Path() const;
	void SetPathL(const TDesC& aPathData);
	void SetType(const CLeafDirTreeNode::TLeafDirTreeNodeType aType);
	inline TBool IsRoot() const;
	inline TBool IsLeaf();
	inline TBool IsLeafIntermediary();
	inline TBool IsPureIntermediary();

private:
	void ConstructL(CLeafDirTree* aOwnerTree, const TDesC& aPathName);
	CLeafDirTreeNode(const TLeafDirData& aDirPos , TLeafDirTreeNodeType aType);
	CLeafDirTreeNode(const CLeafDirTreeNode& aLeafDirTreeNode);
	CLeafDirTreeNode* operator=(CLeafDirTreeNode& aLeafDirTreeNode);

private:
	CLeafDirTree* iOwnerTree;					// A pointer to the owner of this node.
	CLeafDirTreeNode* iParent;					// The parent of this node 
	RPointerArray<CLeafDirTreeNode> iChildren;	// The children of this node
	RBuf iPath;									// The path of the direcotry this node represents
	TLeafDirData iLeafDirData;				    // The position of the direcotry this node represents
	TLeafDirTreeNodeType iNodeType;				// The type of the node
	};

/*
Class definition for the tree structure of the leaf dir cache
All cached direcotries are stored as nodes in a tree structure

*/
class CLeafDirTree : public CBase
	{
public:
	static CLeafDirTree* NewL(TUint32 aSize);
	~CLeafDirTree();

public:
	TInt Search(const TDesC& aPath, CLeafDirTreeNode*& aNodeFound, TLeafDirData& aDirPos); 
	TInt DoSearch(const TDesC& aPath, CLeafDirTreeNode* aNodeToStart, CLeafDirTreeNode*& aNodeFound, TLeafDirData& aDirPos);
	void InsertL(const TDesC& aPath, const TLeafDirData& aDirPos, CLeafDirTreeNode*& aNodeInserted);
	void DoInsertL(CLeafDirTreeNode* aNodeToStart, const TDesC& aPath, const TLeafDirData& aDirPos, CLeafDirTreeNode*& aNodeInserted);
	void RemoveDirL(const TLeafDirData& aDirPos);
	void UpdateMRUPos(const TLeafDirData& aLeafDirData);
	void RemoveFromCacheL(CLeafDirTreeNode* aNodeToDelete);
	CLeafDirTreeNode* FindLeftestLeafNode(CLeafDirTreeNode* aNodeToStart) const;
	void DeleteSubTreeL(CLeafDirTreeNode* aNodeToStart);
	void Reset();
	CLeafDirTreeNode* LruNode();
	void AddOntoLruL(CLeafDirTreeNode* aNodeToAdd);
	TInt RemoveFromLru(CLeafDirTreeNode* aNodeToRemove);
	TInt MakeMostRecentlyUsed(CLeafDirTreeNode* aNodeUsed);
	inline TInt LruCount() const;
	void CheckLimitL();
	
	// For debugging & testing only
	#ifdef _DEBUG
		TInt ObjectCount() const {return iContainer.Count();};
		void AddToObjectContainerL(CLeafDirTreeNode* aNode);
		void RemoveFromObjectContainerL(CLeafDirTreeNode* aNode);
		void DumpTreeContentL() const;
	#endif	//_DEBUG

private:
	void ConstructL();
	CLeafDirTree(TUint32 aSize);

private:
	CLeafDirTreeNode* iRoot;					// The root node
	RPointerArray<CLeafDirTreeNode> iLruList;	// The list containing all the LRU cached nodes
	TUint32 iSize;								// The maximum number of items allowed to cache 

	// For debugging & testing only
	#ifdef _DEBUG
		RPointerArray<CLeafDirTreeNode> iContainer;	// The container containing all nodes
	#endif	//_DEBUG
	};

/*
Class definition for leaf directory cache.
Acting as an interface class for CFatMountCB to use.

*/
class CLeafDirCache : public CBase
	{
public:	
	static CLeafDirCache* NewL(TUint32 aLimit);
	~CLeafDirCache();
	void Reset();
	TInt FindInCache(const TDesC& aLeafDirName, TLeafDirData& aDirPos) const; 
	void AddToCacheL(const TDesC& aLeafDirName, const TLeafDirData& aDirPos);
	void RemoveDirL(const TLeafDirData& aDirPos); 
	void UpdateMRUPos(const TLeafDirData& aLeafDirData);
	TInt CacheCount() const;

	// For debugging & testing only
	#ifdef _DEBUG
		void DumpCacheContentL() const;
		TInt NodeCount() const;
	#endif	//_DEBUG

private:
	CLeafDirCache(TUint32 aLimit);
	void ConstructL();

private:
	TUint32 iSize;		    // The cache size
	CLeafDirTree* iTree;	// The cache content, represented as a tree structure
	};

//---------------------------------------------------------------------------------------------------------------------------------

#include"sl_leafdir_cache.inl"

#endif //SL_LEAFDIR_CACHE_H




