file: Agents/Registry.h
    class CRegistry
    DATA:
        TAgentsHash AgentsHash;
        TAgentsTypeHash AgentsTypeHash;
    BEHAVIOUR:
        CRegistry();
        void Clear();
        void RegisterAgent(string sAgentName, CAgent* pAgent);
        void UnRegisterAgent(string sAgentName);
        bool IsRegisteredAgent(string sAgentName);
        CAgent* operator[](string sAgentName); -> Used by DialogAgents and GroundingActions
        void RegisterAgentType(string sAgentTypeName, FCreateAgent fctCreateAgent);
        void UnRegisterAgentType(string sAgentTypeName);
        bool IsRegisteredAgentType(string sAgentType);
        CAgent* CreateAgent(string sAgentTypeName, string sAgentName, string sAgentConfiguration);

file: Agents/Agent.h
    class CAgent
    DATA:
        string sName;
        string sType;
        STRING2STRING s2sConfiguration;
    BEHAVIOUR:
        CAgent(string sAName, string sAConfiguration, string sAType);
        virtual ~CAgent();
        static CAgent* AgentFactory(string sAName, string sAConfiguration);
        string GetName();
        string GetType();
        void SetConfiguration(string sConfiguration);
        void SetConfiguration(STRING2STRING s2sAConfiguration);
        void SetParameter(string sParam, string sValue);
        bool HasParameter(string sParam);
        string GetParameterValue(string sParam);
        virtual void Register();
        virtual void UnRegister();
        virtual void Create();
        virtual void Initialize();
        virtual void Reset();

file: Agents/DialogAgents/DialogAgent.h
    class CDialogAgent : public CAgent
    DATA:
        string sDialogAgentName;
        TConceptPointersVector Concepts;
        TAgentsVector SubAgents;
        CDialogAgent* pdaParent;
        CDialogAgent* pdaContextAgent;
        CGroundingModel* pGroundingModel;
        bool bCompleted;
        TCompletionType ctCompletionType;
        bool bBlocked;
        bool bDynamicAgent;
        string sDynamicAgentID;
        string sTriggeredByCommands;
        string sTriggerCommandsGroundingModelSpec;
        int iExecuteCounter;
        int iResetCounter;
        int iReOpenCounter;
        int iTurnsInFocusCounter;
        int iLastInputIndex;
        int iLastExecutionInputIndex;
        int iLastExecutionIndex;
        int iLastBindingsIndex;
        STRING2STRING s2sInputLineConfiguration;
        bool bInheritedParentInputConfiguration;
    BEHAVIOUR:
        CDialogAgent(string sAName, string sAConfiguration, string sAType);
        virtual ~CDialogAgent();
        static CAgent* AgentFactory(string sAName, string sAConfiguration);
        virtual void Register();
        virtual void Create();
        virtual void Initialize();
        virtual void Reset();
        virtual void CreateConcepts(); -> defined in DEFINE_CONCEPTS, called in CreateDialogTree diagram
        virtual bool IsExecutable();
        virtual TDialogExecuteReturnCode Execute();
        virtual int DeclareExpectations(TConceptExpectationList& rcelExpectationList);
        virtual void DeclareConcepts(TConceptPointersVector& rcpvConcepts, TConceptPointersSet& rcpsExclude);
        virtual void CreateGroundingModel(string sGroundingModelSpec);
        virtual CGroundingModel* GetGroundingModel();
        virtual void DeclareGroundingModels(TGroundingModelPointersVector& rgmpvModels, TGroundingModelPointersSet& rgmpsExclude);
        virtual bool ExpectCondition();
        virtual string DeclareBindingPolicy(); -> defined in CONCEPT_BINDING_POLICY, used in enforceBindingPolicies
        virtual int DeclareFocusClaims(TFocusClaimsList& fclFocusClaims);
        virtual bool PreconditionsSatisfied();
        virtual bool ClaimsFocus();
        virtual bool ClaimsFocusDuringGrounding();
        virtual string TriggeredByCommands(); -> Defined by TRIGGERED_BY_COMMANDS in each Agent definition, called in CreateDialogTree diagram
        void CreateTriggerConcept();
        virtual bool SuccessCriteriaSatisfied();
        virtual bool FailureCriteriaSatisfied();
        virtual int GetMaxExecuteCounter();
        virtual void OnCreation(); -> defined by ON_CREATION, called in CreateDialogTree diagram
        virtual void OnDestruction(); -> defined by ON_DESTRUCTION, called in DestroyDialogTree in DTTManagerAgent
        virtual void OnInitialization(); -> defined by ON_INITIALIZATION, callied in CreateDialogTree diagram
        virtual void OnCompletion();
        virtual void ReOpen();
        virtual void ReOpenConcepts();
        virtual void ReOpenTopic();
        virtual bool IsAMainTopic();
        virtual string Prompt();
        virtual string TimeoutPrompt();
        virtual string ExplainMorePrompt();
        virtual string CreateVersionedPrompt(string sVersion);
        virtual string EstablishContextPrompt();
        virtual string WhatCanISayPrompt();
        virtual string InputLineConfigurationInitString();
        virtual bool IsDTSAgent();
        virtual bool IsConversationSynchronous();
        virtual bool RequiresFloor();
        virtual void Undo();
        virtual CConcept& C(string sConceptPath);
        virtual CConcept& C(const char* lpszConceptPath, ...);
        virtual CConcept& LocalC(string sConceptName);
        CDialogAgent& A(string sDialogAgentPath);
        CDialogAgent& A(const char* lpszDialogAgentPath, ...);
        void AddSubAgent(CDialogAgent* pdaWho, CDialogAgent* pdaWhere, TAddSubAgentMethod asamMethod);
        void DeleteSubAgent(CDialogAgent* pdaWho);
        void DeleteDynamicSubAgents();
        void SetParent(CDialogAgent* pdaAParent);
        CDialogAgent* GetParent();
        void UpdateName();
        void SetContextAgent(CDialogAgent* pdaAContextAgent);
        CDialogAgent* GetContextAgent();
        CDialogAgent* GetMainTopic();
        bool HasCompleted();
        void SetCompleted(TCompletionType ctACompletionType = ctSuccess);
        void ResetCompleted();
        bool HasFailed();
        bool HasSucceeded();
        bool IsAgentPathBlocked();
        virtual bool IsBlocked();
        void Block();
        void UnBlock();
        void SetDynamicAgent();
        bool IsDynamicAgent();
        void SetDynamicAgentID(string sADynamicAgentID);
        string GetDynamicAgentID();
        void IncrementExecuteCounter();
        int GetExecuteCounter();
        bool WasReset();
        bool WasReOpened();
        void IncrementTurnsInFocusCounter();
        int GetTurnsInFocusCounter();
        void SetLastInputIndex(int iInputIndex);
        int GetLastInputIndex();
        void SetLastExecutionInputIndex(int iInputIndex);
        int GetLastExecutionInputIndex();
        void SetLastExecutionIndex(int iExecutionIndex);
        int GetLastExecutionIndex();
        void SetLastBindingsIndex(int iBindingsIndex);
        int GetLastBindingsIndex();
        void SetInputConfiguration( string sConfiguration);
        void SetInputConfigurationParameter( string sSlot, string sValue);
        bool HasInputConfigurationParameter( string sSlot);
        string GetInputConfigurationParameterValue( string sSlot);
        void InheritParentInputConfiguration();
        STRING2STRING GetInputLineConfiguration();
        void parseGrammarMapping(string sConceptNames, string sGrammarMapping, TConceptExpectationList& rcelExpectationList);

file: Concepts/Concept.h
    class CHyp
    DATA:
        TConceptType ctHypType;
        float fConfidence;
    BEHAVIOUR:
        CHyp();
        CHyp(CHyp& rAHyp);
        virtual ~CHyp();
        TConceptType GetHypType();
        float GetConfidence();
        virtual CHyp& operator = (CHyp& rAHyp);
        virtual void SetConfidence(float fAConfidence);
        virtual bool operator == (CHyp& rAHyp);
        virtual bool operator < (CHyp& rAHyp);
        virtual bool operator > (CHyp& rAHyp);
        virtual bool operator <= (CHyp& rAHyp);
        virtual bool operator >= (CHyp& rAHyp);
        virtual CHyp* operator [](string sItem);
        virtual string ValueToString();
        virtual string ToString();
        virtual void FromString(string sString);

    class CConcept
    DATA:
        TConceptType ctConceptType;
        TConceptSource csConceptSource;
        string sName;
        CDialogAgent* pOwnerDialogAgent;
        CConcept* pOwnerConcept;
        CGMConcept* pGroundingModel;
        bool bGrounded;
        bool bInvalidated;
        bool bRestoredForGrounding;
        bool bSealed;
        bool bChangeNotification;
        vector<CHyp*, allocator<CHyp*>> vhCurrentHypSet;
        int iNumValidHyps;
        vector<CHyp*, allocator<CHyp*>> vhPartialHypSet;
        int iNumValidPartialHyps;
        int iCardinality;
        int iTurnLastUpdated;
        TConveyance cConveyance;
        bool bWaitingConveyance;
        CConcept* pPrevConcept;
        bool bHistoryConcept;
        string sExplicitlyConfirmedHyp;
        string sExplicitlyDisconfirmedHyp;
    BEHAVIOUR:
        CConcept(string sAName, TConceptSource csAConceptSource, int iACardinality);
        virtual ~CConcept();
        virtual CConcept& operator = (CConcept& rAConcept);
        virtual CConcept& operator = (int iAValue);
        virtual CConcept& operator = (bool bAValue);
        virtual CConcept& operator = (float fAValue);
        virtual CConcept& operator = (double dAValue);
        virtual CConcept& operator = (string sAValue);
        virtual CConcept& operator = (const char* lpszAValue);
        virtual bool operator == (CConcept& rAConcept);
        virtual bool operator != (CConcept& rAConcept);
        virtual bool operator < (CConcept& rAConcept);
        virtual bool operator > (CConcept& rAConcept);
        virtual bool operator <= (CConcept& rAConcept);
        virtual bool operator >= (CConcept& rAConcept);
        virtual CConcept& operator [](int iIndex);
        virtual CConcept& operator [](string sIndex);
        virtual CConcept& operator [](const char* lpszIndex);
        virtual operator int();
        virtual operator float();
        virtual operator bool();
        virtual operator string();
        virtual void Clear();
        virtual void ClearCurrentValue();
        virtual CConcept* Clone(bool bCloneHistory);
        virtual CConcept* EmptyClone();
        virtual void Update(string sUpdateType, void* pUpdateData);
        virtual void Update_NPU_AssignFromString(void* pUpdateData);
        virtual void Update_NPU_AssignFromConcept(void* pUpdateData);
        virtual void Update_NPU_UpdateWithConcept(void* pUpdateData);
        virtual void Update_NPU_CollapseToMode(void* pUpdateData);
        virtual void Update_PartialFromString(void* pUpdateData);
        virtual void Update_Calista_AssignFromString(void* pUpdateData);
        virtual void Update_Calista_AssignFromConcept(void* pUpdateData);
        virtual void Update_Calista_UpdateWithConcept(void* pUpdateData);
        virtual void Update_Calista_CollapseToMode(void* pUpdateData);
        virtual bool IsUpdated();
        virtual bool IsUpdatedAndGrounded();
        virtual bool IsAvailable();
        virtual bool IsAvailableAndGrounded();
        virtual bool HasPartialUpdate();
        virtual bool IsGrounded();
        virtual bool IsUndergoingGrounding();
        virtual bool IsInvalidated();
        virtual bool IsRestoredForGrounding();
        virtual string GroundedHypToString();
        virtual string TopHypToString();
        virtual string HypSetToString();
        virtual TConceptType GetConceptType();
        virtual void SetConceptType(TConceptType ctAConceptType);
        virtual TConceptSource GetConceptSource();
        virtual void SetConceptSource(TConceptSource csAConceptSource);
        virtual void SetName(string sAName);
        string GetName();
        string GetSmallName();
        string GetAgentQualifiedName();
        virtual void SetOwnerDialogAgent(CDialogAgent* pADialogAgent); -> called in CreateDialogTree by the TRIGGERED_BY_COMMANDS definition
        CDialogAgent* GetOwnerDialogAgent();
        virtual void SetOwnerConcept(CConcept* pAConcept);
        CConcept* GetOwnerConcept();
        virtual void CreateGroundingModel(string sGroundingModelSpec); -> called in CreateDialogTree by the TRIGGERED_BY_COMMANDS definition
        CGMConcept* GetGroundingModel();
        virtual void SetGroundedFlag(bool bAGrounded);
        virtual bool GetGroundedFlag();
        virtual void DeclareGroundingModels(TGroundingModelPointersVector& rgmpvModels, TGroundingModelPointersSet& rgmpsExclude);
        virtual void DeclareConcepts(TConceptPointersVector& rcpvConcepts, TConceptPointersSet& rcpsExclude);
        virtual void SetInvalidatedFlag(bool bAInvalidated);
        virtual bool GetInvalidatedFlag();
        virtual void SetRestoredForGroundingFlag(bool bARestoredForGrounding);
        virtual bool GetRestoredForGroundingFlag();
        virtual void Seal();
        virtual void BreakSeal();
        virtual bool IsSealed(); used in GroundingActions
        virtual void DisableChangeNotification();
        virtual void EnableChangeNotification();
        virtual void SetChangeNotification(bool bAChangeNotification);
        virtual void NotifyChange();
        virtual CHyp* HypFactory();
        virtual int AddHyp(CHyp* pAHyp);
        virtual int AddNewHyp();
        virtual int AddNullHyp();
        virtual void SetHyp(int iIndex, CHyp* pHyp);
        virtual void SetNullHyp(int iIndex);
        virtual void DeleteHyp(int iIndex);
        virtual CHyp* GetHyp(int iIndex);
        virtual int GetHypIndex(CHyp* pHyp);
        virtual float GetHypConfidence(int iIndex);
        virtual void SetHypConfidence(int iIndex, float fConfidence);
        virtual CHyp* GetTopHyp();
        virtual int GetTopHypIndex();
        virtual int Get2ndHypIndex();
        virtual float GetTopHypConfidence();
        virtual bool IsValidHyp(int iIndex);
        virtual int GetNumHyps();
        virtual int GetNumValidHyps();
        virtual void ClearCurrentHypSet();
        virtual void CopyCurrentHypSetFrom(CConcept& rAConcept);
        virtual void SetCardinality(int iACardinality);
        virtual int GetCardinality();
        virtual float GetPriorForHyp(CHyp* pHyp);
        virtual float GetConfusabilityForHyp(CHyp* pHyp);
        virtual string GetConceptTypeInfo();
        virtual void SetExplicitlyConfirmedHyp(CHyp* pHyp);
        virtual void SetExplicitlyConfirmedHyp(string sAExplicitlyConfirmedHyp);
        virtual void SetExplicitlyDisconfirmedHyp(CHyp* pHyp);
        virtual void SetExplicitlyDisconfirmedHyp(string sAExplicitlyDisconfirmedHyp);
        virtual string GetExplicitlyConfirmedHypAsString();
        virtual string GetExplicitlyDisconfirmedHypAsString();
        virtual void ClearExplicitlyConfirmedHyp();
        virtual void ClearExplicitlyDisconfirmedHyp();
        virtual int AddPartialHyp(CHyp* pAHyp);
        virtual int AddNewPartialHyp();
        virtual int AddNullPartialHyp();
        virtual bool HasPartialHyp();
        virtual CHyp* GetPartialHyp(int iIndex);
        virtual CHyp* GetTopPartialHyp();
        virtual int GetTopPartialHypIndex();
        virtual float GetTopPartialHypConfidence();
        bool IsValidPartialHyp(int iIndex);
        virtual int GetNumPartialHyps();
        virtual int GetNumValidPartialHyps();
        virtual void ClearPartialHypSet();
        virtual void SetTurnLastUpdated(int iTurn);
        virtual void MarkTurnLastUpdated();
        virtual int GetTurnLastUpdated();
        int GetTurnsSinceLastUpdated();
        void SetWaitingConveyance();
        void ClearWaitingConveyance();
        virtual void SetConveyance(TConveyance cConveyance);
        TConveyance GetConveyance();
        virtual void ClearConceptNotificationPointer();
        virtual void ReOpen();
        virtual void Restore(int iIndex);
        virtual void ClearHistory();
        virtual CConcept* CreateMergedHistoryConcept();
        virtual void MergeHistory();
        int GetHistorySize();
        virtual CConcept& GetHistoryVersion(int iIndex);
        virtual void SetHistoryConcept(bool bAHistoryConcept);
        virtual bool IsHistoryConcept();
        virtual int GetSize();
        virtual void DeleteAt(unsigned int iIndex);
        virtual void InsertAt(unsigned int iIndex, CConcept &rAConcept);

file: Agents/CoreAgents/DMCoreAgent.h
    class CDMCoreAgent: public CAgent
        friend class CGroundingManagerAgent;
        friend class CStateManagerAgent;
    DATA:
        TExecutionStack esExecutionStack;
        CExecutionHistory ehExecutionHistory;
        TBindingHistory bhBindingHistory;
        TExpectationAgenda eaAgenda;
        TFocusClaimsList fclFocusClaims;
        TSystemAction saSystemAction;
        int iTimeoutPeriod;
        int iDefaultTimeoutPeriod;
        float fNonunderstandingThreshold;
        float fDefaultNonunderstandingThreshold;
        STRING2BFF s2bffFilters;
        bool bFocusClaimsPhaseFlag;
        bool bAgendaModifiedFlag;
        TFloorStatus fsFloorStatus;
        int iTurnNumber;
        TCustomStartOverFunct csoStartOverFunct;
    BEHAVIOUR:
        CDMCoreAgent(string sAName, string sAConfiguration, string sAType);
        virtual ~CDMCoreAgent();
        static CAgent* AgentFactory(string sAName, string sAConfiguration);
        virtual void Reset();
        void Execute();
        void AcquireNextEvent();
        void RegisterBindingFilter(string sBindingFilterName, TBindingFilterFunct bffFilter);
        int GetIntSessionID();
        void SetTimeoutPeriod(int iATimeoutPeriod);
        int GetTimeoutPeriod();
        void SetDefaultTimeoutPeriod(int iADefaultTimeoutPeriod);
        int GetDefaultTimeoutPeriod();
        void SetNonunderstandingThreshold(float fANonunderstandingThreshold);
        float GetNonunderstandingThreshold();
        void SetDefaultNonunderstandingThreshold(float fANonuThreshold);
        float GetDefaultNonunderstandingThreshold();
        void SignalFocusClaimsPhase(bool bAFocusClaimsPhaseFlag);
        void SetFloorStatus(TFloorStatus fsaFloorStatus);
        TFloorStatus GetFloorStatus();
        string FloorStatusToString(TFloorStatus fsAFloor);
        TFloorStatus StringToFloorStatus(string sAFloor);
        int LastTurnGetConceptsBound();
        bool LastTurnNonUnderstanding(); -> Used by GroundingActions, GMRequestAgent and DialogAgents
        int GetNumberNonUnderstandings();
        int GetTotalNumberNonUnderstandings();
        void ContinueWith(CAgent* paPusher, CDialogAgent* pdaDialogAgent); -> GroundingActions and both types of DialogAgents
        void RestartTopic(CDialogAgent* pdaDialogAgent);
        void RegisterCustomStartOver(TCustomStartOverFunct csoAStartOverFunct); -> defined in CUSTOM_START_OVER
        void StartOver();
        CDialogAgent* GetAgentInFocus();
        CDialogAgent* GetDTSAgentInFocus();
        bool AgentIsInFocus(CDialogAgent* pdaDialogAgent);
        CDialogAgent* GetAgentPreviouslyInFocus(CDialogAgent* pdaDialogAgent);
        CDialogAgent* GetDTSAgentPreviouslyInFocus( CDialogAgent* pdaDialogAgent);
        CDialogAgent* GetCurrentMainTopicAgent();
        bool AgentIsActive(CDialogAgent* pdaDialogAgent); -> used by IS_ACTIVE_TOPIC
        void PopAgentFromExecutionStack(CDialogAgent* pdaADialogAgent);
        void PopTopicFromExecutionStack(CDialogAgent* pdaADialogAgent);
        void PopGroundingAgentsFromExecutionStack();
        int GetBindingHistorySize();
        const TBindingsDescr& GetBindingResult(int iBindingHistoryIndex);
        int GetLastInputTurnNumber();
        TSystemActionOnConcept GetSystemActionOnConcept(CConcept* pConcept);
        void SignalExplicitConfirmOnConcept(CConcept* pConcept);
        void SignalImplicitConfirmOnConcept(CConcept* pConcept);
        void SignalUnplannedImplicitConfirmOnConcept(int iState, CConcept* pConcept);
        void SignalAcceptOnConcept(CConcept* pConcept);
        int popCompletedFromExecutionStack();
        void popAgentFromExecutionStack(CDialogAgent *pdaADialogAgent, TStringVector& rvsAgentsEliminated);
        void popTopicFromExecutionStack(CDialogAgent *pdaADialogAgent, TStringVector& rvsAgentsEliminated);
        void popGroundingAgentsFromExecutionStack(TStringVector& rvsAgentsEliminated);
        void dumpConcepts();
        void dumpExecutionStack();
        string executionStackToString();
        string executionStackToString(TExecutionStack es);
        void clearCurrentSystemAction();
        void computeCurrentSystemAction();
        string systemActionToString(TSystemAction saASystemAction);
        string currentSystemActionToString();
        void assembleExpectationAgenda();
        void compileExpectationAgenda();
        void enforceBindingPolicies();
        void broadcastExpectationAgenda();
        string expectationAgendaToString();
        string expectationAgendaToBroadcastString();
        string expectationAgendaToBroadcastString(TExpectationAgenda eaBAgenda);
        string bindingsDescrToString(TBindingsDescr& rbdBindings);
        void updateInputLineConfiguration();
        int assembleFocusClaims();
        void bindConcepts(TBindingsDescr& rbdBindings);
        void performConceptBinding(string sSlotName, string sSlotValue, float fConfidence, int iExpectationIndex, bool bIsComplete);
        void performCustomConceptBinding(int iExpectationIndex);
        void performForcedConceptUpdates(TBindingsDescr& rbdBindings);
        void processNonUnderstanding();
        void resolveFocusShift();
        void rollBackDialogState(int iState);

file: Agents/CoreAgents/DTTManagerAgent.h
    class CDTTManagerAgent: public CAgent
    DATA:
        CDialogAgent* pdaDialogTaskRoot;
        vector<TDiscourseAgentInfo, allocator<TDiscourseAgentInfo>> vdaiDAInfo;
    BEHAVIOUR:
        CDTTManagerAgent(string sAName, string sAConfiguration, string sAType);
        virtual ~CDTTManagerAgent();
        static CAgent* AgentFactory(string sAName, string sAConfiguration);
        virtual void Reset();
        void Use(string sDAType, string sDAName, FRegisterAgent fRegisterAgent, string sConfiguration);
        void CreateDialogTree();
        void DestroyDialogTree();
        void ReCreateDialogTree();
        void CreateDialogTaskTree(); -> DECLARE_DIALOG_TASK_ROOT, called by CreateDialogTree
        void CreateDialogTaskAgentome(); -> DECLARE_AGENTS, called by CreateDialogTree
        CDialogAgent* GetDialogTaskTreeRoot();
        void MountAgent(CDialogAgent* pdaWhere, CDialogAgent* pdaWho, TMountingMethod mmHow, string sDynamicAgentID);
        CDialogAgent* MountAgent(CDialogAgent* pWhere, string sAgentType, string sAgentName, string sAConfiguration, TMountingMethod mmHow, string sDynamicAgentID);
        void UnMountAgent(CDialogAgent* pdaWho);
        void MountAgentsFromArrayConcept(CDialogAgent *pdaParent, string sArrayConceptName, string sAgentsType, string sAgentsName, string sAgentsConceptName, string sAgentsDynamicID);
        string GetParentName(string sAgentPath);
        bool IsParentOf(string sParentAgentPath, string sAgentPath);
        bool IsChildOf(string sChildAgentPath, string sAgentPath);
        bool IsAncestorOf(string sAncestorAgentPath, string sAgentPath);
        bool IsAncestorOrEqualOf(string sAncestorAgentPath, string sAgentPath);
        bool IsDescendantOf(string sDescendantAgentPath, string sAgentPath);
        bool IsSiblingOf(string sAgent1Path, string sAgent2Path);

file: Agents/CoreAgents/GroundingManagerAgent.h
    class CGroundingManagerAgent: public CAgent
    DATA:
        STRING2STRING s2sPolicies;
        STRING2FLOAT s2fConstantParameters;
        STRING2STRING2FLOATVECTOR s2s2vfBeliefUpdatingModels;
        STRING2STRING2FLOATVECTOR s2s2vfConceptValuesInfo;
        STRING2STRING s2sConceptTypeInfo;
        STRING2FLOAT s2fBeliefUpdatingFeatures;
        vector < CGroundingAction * > vpgaActions;
        TStringVector vsActionNames;
        TExternalPolicies mapExternalPolicies;
        TGroundingManagerConfiguration gmcConfig;
        bool bTurnGroundingRequest;
        TConceptGroundingRequests vcgrConceptGroundingRequests;
        bool bLockedGroundingRequests;
        vector<TGroundingActionHistoryItems> vgahiGroundingActionsHistory;
        TGroundingModelsTypeHash gmthGroundingModelTypeRegistry;
    BEHAVIOUR:
        CGroundingManagerAgent(string sAName, string sAConfiguration, string sAType);
        virtual ~CGroundingManagerAgent();
        static CAgent* AgentFactory(string sAName, string sAConfiguration);
        void SetConfiguration(string sAGroundingManagerConfiguration);
        TGroundingManagerConfiguration GetConfiguration();
        virtual void LoadPoliciesFromString(string sPolicies);
        virtual void LoadPoliciesFromFile(string sFileName);
        virtual string GetPolicy(string sModelName);
        CExternalPolicyInterface* CreateExternalPolicyInterface(string sAHost);
        void ReleaseExternalPolicyInterfaces();
        virtual void LoadBeliefUpdatingModel(string sAFileName);
        virtual void SetBeliefUpdatingModelName(string sABeliefUpdatingModelName);
        virtual string GetBeliefUpdatingModelName();
        virtual STRING2FLOATVECTOR& GetBeliefUpdatingModelForAction(string sSystemAction);
        virtual float GetConstantParameter(string sParam);
        virtual void PrecomputeBeliefUpdatingFeatures(CConcept* pIConcept, CConcept* pNewConcept, string sSystemAction);
        virtual float GetGroundingFeature(string sFeatureName);
        virtual string GetGroundingFeatureAsString(string sFeatureName);
        virtual void ClearBeliefUpdatingFeatures();
        virtual float GetPriorForConceptHyp(string sConcept, string sHyp);
        virtual float GetConfusabilityForConceptHyp(string sConcept, string sHyp);
        virtual string GetConceptTypeInfoForConcept(string sConcept);
        void UseGroundingAction(string sActionName, GroundingAction* pgaAGroundingAction);
        int GroundingActionNameToIndex(string sGroundingActionName);
        string GroundingActionIndexToName(unsigned int iGroundingActionIndex);
        CGroundingAction* operator[] (string sGroundingActionName);
        CGroundingAction* operator[] (unsigned int iGroundingActionIndex);
        void RequestTurnGrounding(bool bATurnGroundingRequest);
        void RequestConceptGrounding(CConcept* pConcept);
        string ScheduleConceptGrounding(CConcept* pConcept);
        void LockConceptGroundingRequestsQueue();
        void UnlockConceptGroundingRequestsQueue();
        void SetConceptGroundingRequestStatus(CConcept* pConcept, int iAGroundingRequestStatus);
        int GetConceptGroundingRequestStatus(CConcept* pConcept);
        void ConceptGroundingRequestCompleted(CConcept* pConcept);
        void RemoveConceptGroundingRequest(CConcept* pConcept);
        void PurgeConceptGroundingRequestsQueue();
        void GAHAddHistoryItem(string sGroundingModelName, string sActionName, int iGroundingActionType);
        void GAHSetBargeInFlag(int iTurnNum, bool bBargeInFlag);
        string GAHGetTurnGroundingAction(int iTurnNumber);
        int GAHCountTakenInLastNTurns(bool bAlsoHeard, string sActionName, int iNumTurns);
        int GAHCountTakenByGroundingModelInLastNTurns(bool bAlsoHeard, string sActionName, string sGroundingModelName, int iNumTurns);
        void RegisterGroundingModelType(string sName, FCreateGroundingModel fctCreateGroundingModel);
        CGroundingModel* CreateGroundingModel(string sModelType, string sModelPolicy);
        bool HasPendingRequests();
        bool HasPendingTurnGroundingRequest();
        bool HasPendingConceptGroundingRequests();
        bool HasUnprocessedConceptGroundingRequests();
        bool HasScheduledConceptGroundingRequests();
        bool HasExecutingConceptGroundingRequests();
        bool GroundingInProgressOnConcept(CConcept* pConcept);
        string GetScheduledGroundingActionOnConcept(CConcept* pConcept);
        virtual void Run();
        int getConceptGroundingRequestIndex(CConcept* pConcept);
        string loadPolicy(string sFileName);

file: Grounding/GroundingModels/GroundingModel.h
    class CGroundingModel
    DATA:
        string sName;
        string sModelPolicy;
        TPolicy pPolicy;
        bool bExternalPolicy;
        CExternalPolicyInterface* pepiExternalPolicy;
        string sExternalPolicyHost;
        vector<int> viActionMappings;
        string sExplorationMode;
        float fExplorationParameter;
        CState stFullState;
        CBeliefDistribution bdBeliefState;
        CBeliefDistribution bdActionValues;
        int iSuggestedActionIndex;
    BEHAVIOUR:
        CGroundingModel(string sAModelPolicy, string sAModelName);
        CGroundingModel(CGroundingModel& rAGroundingModel);
        virtual ~CGroundingModel();
        static CGroundingModel* GroundingModelFactory(string sModelPolicy);
        virtual string GetType();
        virtual string GetModelPolicy();
        virtual string GetName();
        virtual void SetName(string sAName);
        virtual void Initialize();
        virtual CGroundingModel* Clone() = 0;
        virtual bool LoadPolicy();
        virtual void ComputeState();
        virtual void ComputeActionValuesDistribution();
        virtual int ComputeSuggestedActionIndex();
        virtual void Run();
        virtual void RunAction(int iActionIndex);
        virtual void LogStateAction();
        virtual void computeFullState() = 0;
        virtual void computeBeliefState() = 0;
        string beliefStateToString();
        string actionValuesToString();

file: Grounding/GroundingActions/GroundingAction.h
    class CGroundingAction
    DATA:
        STRING2STRING s2sConfiguration;
    BEHAVIOUR:
        CGroundingAction();
        CGroundingAction(string sNewConfiguration);
        virtual ~CGroundingAction();
        virtual string GetName() = 0;
        void SetConfiguration(string sNewConfiguration);
        string GetConfiguration();
        virtual void Run(void *pParams) = 0;
        virtual void RegisterDialogAgency() = 0;

file: Grounding/GroundingUtils.h
    class CState
    DATA:
        STRING2STRING s2sStateVars;
    BEHAVIOUR:
        string& operator[] (string sIndex);
        void Clear();
        void Add(STRING2STRING s2s);
        string ToString();

    class CBeliefDistribution
    DATA:
        vector<float> vfProbability;
        vector<float> vfProbabilityLowBound;
        vector<float> vfProbabilityHiBound;
    BEHAVIOUR:
        CBeliefDistribution(int iNumEvents);
        CBeliefDistribution(CBeliefDistribution& rbdABeliefDistribution);
        ~CBeliefDistribution();
        void Resize(int iNumEvents);
        float& operator[] (unsigned int iIndex);
        float& LowBound(unsigned int iIndex);
        float& HiBound(unsigned int iIndex);
        int GetValidEventsNumber();
        void Normalize();
        void Sharpen(float fPower);
        int GetModeEvent();
        int GetMaxHiBoundEvent();
        int GetEpsilonGreedyEvent(float fEpsilon);
        int GetSoftMaxEvent(float fTemperature);
        int GetRandomlyDrawnEvent();

file: Agents/CoreAgents/StateManagerAgent.h
    class CStateManagerAgent : public CAgent
    DATA:
        STRING2STRING s2sDialogStateNames;
        vector<TDialogState, allocator<TDialogState>> vStateHistory;
        string sStateBroadcastAddress;
    BEHAVIOUR:
        CStateManagerAgent(string sAName, string sAConfiguration, string sAType);
        virtual ~CStateManagerAgent();
        static CAgent* AgentFactory(string sAName, string sAConfiguration);
        virtual void Reset();
        void LoadDialogStateNames(string sFileName);
        void SetStateBroadcastAddress(string sAStateBroadcastAddress);
        void BroadcastState();
        void UpdateState();
        string GetStateAsString(TDialogState dsState);
        string GetStateAsString();
        int GetStateHistoryLength();
        TDialogState &GetLastState();
        TDialogState &operator[](unsigned int i);

file: Agents/CoreAgents/InteractionEventManagerAgent.h
    class CInteractionEventManagerAgent: public CAgent
    DATA:
        HANDLE hNewInteractionEvent;
        queue <CInteractionEvent*, list<CInteractionEvent*>> qpieEventQueue;
        vector <CInteractionEvent*> vpieEventHistory;
        CInteractionEvent *pieLastEvent;
        CInteractionEvent *pieLastInput;
    BEHAVIOUR:
        CInteractionEventManagerAgent(string sAName, string sAConfiguration, string sAType);
        virtual ~CInteractionEventManagerAgent();
        static CAgent* AgentFactory(string sAName, string sAConfiguration);
        virtual void Reset();
        virtual void Initialize();
        bool HasEvent();
        CInteractionEvent *GetNextEvent();
        CInteractionEvent *GetLastEvent(); -> used by DateTimeBindingFilters in the DialogTask
        CInteractionEvent *GetLastInput(); -> used by the Grounding Models
        bool LastEventMatches(string sGrammarExpectation);
        bool LastInputMatches(string sGrammarExpectation);
        bool LastEventIsComplete();
        float GetLastEventConfidence();
        string GetValueForExpectation(string sGrammarExpectation);
        void WaitForEvent();
        void SignalInteractionEventArrived();

file: Events/GalaxyInteractionEvent.h
    class CGalaxyInteractionEvent: public CInteractionEvent
    DATA:
        void  *gfEvent;
    BEHAVIOUR:
        CGalaxyInteractionEvent();
        CGalaxyInteractionEvent(void *gfAEvent);
        ~CGalaxyInteractionEvent();
        void *GetEventFrame();

    class CInteractionEvent
    DATA:
        string sType;
        int iID;
        bool bComplete;
        float fConfidence;
        STRING2STRING s2sProperties;
    BEHAVIOUR:
        CInteractionEvent();
        CInteractionEvent(string sAType);
        virtual ~CInteractionEvent();
        string GetType();
        int GetID();
        bool IsComplete();
        float GetConfidence();
        string GetStringProperty(string sSlot);
        int GetIntProperty(string sSlot);
        float GetFloatProperty(string sSlot);
        bool HasProperty(string sSlot);
        void SetProperty(string sSlot, string sValue);
        STRING2STRING &GetProperties();
        string ToString();
        bool Matches(string sGrammarExpectation);
        string GetValueForExpectation(string sGrammarExpectation);
        bool matchesSlot( string slot, string pattern);

file: Agents/CoreAgents/OutputManagerAgent.h
    class COutputHistory
    DATA:
        vector<string> vsUtterances;
        vector<COutput*> vopOutputs;
        unsigned int uiCurrentID;
    BEHAVIOUR:
        COutputHistory();
        virtual ~COutputHistory();
        string ToString();
        unsigned int AddOutput(COutput* pOutput, string sUtterance);
        void Clear();
        unsigned int GetSize();
        string GetUtteranceAt(unsigned int iIndex);
        COutput* GetOutputAt(unsigned int iIndex);
        COutput* operator[](unsigned int iIndex);

    class COutputManagerAgent : public CAgent
    DATA:
        COutputHistory ohHistory;
        vector <TOutputDevice> vodOutputDevices;
        unsigned int iDefaultOutputDevice;
        vector <COutput *> vopRecentOutputs;
        int iOutputCounter;
        CRITICAL_SECTION csCriticalSection;
        string sOutputClass;
    BEHAVIOUR:
        COutputManagerAgent(string sAName, string sAConfiguration, string sAType);
        virtual ~COutputManagerAgent();
        static CAgent* AgentFactory(string sAName, string sAConfiguration);
        virtual void Reset();
        void SetOutputClass(string sAOutputClass);
        bool RegisterOutputDevice(string sName, string sServerCall, int iParams);
        void SetDefaultOutputDevice(string sName);
        TOutputDevice *GetOutputDevice(string sName);
        TOutputDevice *GetDefaultOutputDevice();
        string GetDefaultOutputDeviceName();
        vector<COutput*> Output(CDialogAgent* pGeneratorAgent, string sPrompts, TFloorStatus fsFinalFloorStatus);
        void Repeat();
        void Notify(int iOutputId, int iBargeinPos, string sConveyance, string sTaggedUtt);
        void PreliminaryNotify(int iOutputId, string sTaggedUtt);
        void CancelConceptNotificationRequest(CConcept* pConcept);
        void ChangeConceptNotificationPointer(CConcept* pOldConcept, CConcept* pNewConcept);
        string GetPromptsWaitingForNotification();
        string output(COutput* pOutput);
        unsigned int getRecentOutputIndex(int iConceptId);

file: Outputs/Output.h
    class COutput
        friend class COutputHistory;
        friend class COutputManagerAgent;
    DATA:
        string sGeneratorAgentName;
        int iOutputId;
        int iExecutionIndex;
        string sDialogState;
        string sAct;
        string sObject;
        vector<CConcept *> vcpConcepts;
        vector<bool> vbNotifyConcept;
        vector<string> vsFlags;
        string sOutputDeviceName;
        TConveyance cConveyance;
        int iRepeatCounter;
        TFloorStatus fsFinalFloorStatus;
    BEHAVIOUR:
        COutput();
        virtual ~COutput();
        virtual bool Create(string sAGeneratorAgentName, int iAExecutionIndex, string sAPrompt, TFloorStatus fsAFloor, int iAOutputId) = 0;
        virtual string ToString() = 0;
        virtual COutput* Clone(int iNewOutputId) = 0;
        string GetGeneratorAgentName();
        int GetDialogStateIndex();
        void SetDialogStateIndex(int iAExecutionIndex);
        string GetDialogState();
        void SetDialogState(string sADialogState);
        void SetConveyance(TConveyance cAConveyance);
        TConveyance GetConveyance();
        void SetAct(string sAAct);
        string GetAct();
        void SetFinalFloorStatus(TFloorStatus fsAFloor);
        TFloorStatus GetFinalFloorStatus();
        string GetFinalFloorStatusLabel();
        bool CheckFlag(string sFlag);
        void NotifyConceptConveyance(string sConceptName, TConveyance cAConveyance);
        virtual CConcept* GetConceptByName(string sConceptName);
        void CancelConceptNotificationRequest(CConcept* pConcept);
        void ChangeConceptNotificationPointer(CConcept* pOldConcept, CConcept* pNewConcept);
        int GetRepeatCounter();
        void IncrementRepeatCounter();
        virtual void clone(COutput* pOutput, int iNewOutputId);

file: Agents/CoreAgents/TrafficManagerAgent.h
    class CTrafficManagerAgent : public CAgent
    BEHAVIOUR:
        CTrafficManagerAgent(string sAName, string sAConfiguration, string sAType);
        virtual ~CTrafficManagerAgent();
        static CAgent* AgentFactory(string sAName, string sAConfiguration);
        void Call(CDialogAgent* pCallerAgent, string sCall);
        void galaxyCall(TExternalCallSpec ecsCall);
        void oaaCall(TExternalCallSpec ecsCall);
