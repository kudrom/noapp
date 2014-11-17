file: Agents/Registry.h
    class CRegistry
    DATA:
        TAgentsHash AgentsHash;
        TAgentsTypeHash AgentsTypeHash;
    BEHAVIOUR:
        CRegistry();
        void Clear();
        void RegisterAgent(string sAgentName, CAgent* pAgent); -> CreateDialogTree diagram
        void UnRegisterAgent(string sAgentName);
        bool IsRegisteredAgent(string sAgentName);
        CAgent* operator[](string sAgentName); -> Used by DMCoreAgent, GroundingManagerAgent, Concept and Outputs
        void RegisterAgentType(string sAgentTypeName, FCreateAgent fctCreateAgent); -> CreateDialogTree diagram
        void UnRegisterAgentType(string sAgentTypeName);
        bool IsRegisteredAgentType(string sAgentType);
        CAgent* CreateAgent(string sAgentTypeName, string sAgentName, string sAgentConfiguration); -> CreateDialogTree diagram

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
        int iTurnsInFocusCounter; -> only used by Update_NPU_UpdateWithConcept
        int iLastInputIndex;
        int iLastExecutionInputIndex;
        int iLastExecutionIndex;
        int iLastBindingsIndex;
        STRING2STRING s2sInputLineConfiguration;
        bool bInheritedParentInputConfiguration;
    BEHAVIOUR:
        CDialogAgent(string sAName, string sAConfiguration, string sAType);
        virtual ~CDialogAgent();
        static CAgent* AgentFactory(string sAName, string sAConfiguration); -> CreateDialogTree diagram
        virtual void Register(); -> CreateDialogTree diagram
        virtual void Create(); -> CreateDialogTree diagram
        virtual void Initialize() -> CreateDialogTree diagram;
        virtual void Reset();
        virtual void CreateConcepts(); -> CreateDialogTree diagram
        virtual bool IsExecutable();
        virtual TDialogExecuteReturnCode Execute(); -> DMCore thread diagram
        virtual int DeclareExpectations(TConceptExpectationList& rcelExpectationList); -> called in compileExpectationAgenda
        virtual void DeclareConcepts(TConceptPointersVector& rcpvConcepts, TConceptPointersSet& rcpsExclude);
        virtual void CreateGroundingModel(string sGroundingModelSpec);
        virtual CGroundingModel* GetGroundingModel(); -> Used by GroundingManagerAgent
        virtual void DeclareGroundingModels(TGroundingModelPointersVector& rgmpvModels, TGroundingModelPointersSet& rgmpsExclude);
        virtual bool ExpectCondition();
        virtual string DeclareBindingPolicy();
        virtual int DeclareFocusClaims(TFocusClaimsList& fclFocusClaims); -> used by DMCoreAgent's assembleFocusClaims
        virtual bool PreconditionsSatisfied();
        virtual bool ClaimsFocus(); used only by DialogAgent in DeclareFocusClaims
        virtual bool ClaimsFocusDuringGrounding();
        virtual string TriggeredByCommands();
        void CreateTriggerConcept(); -> CreateDialogTree diagram
        virtual bool SuccessCriteriaSatisfied(); -> defined by subclasses, used by assembleFocusClaims
        virtual bool FailureCriteriaSatisfied(); -> defined by subclasses, used by assembleFocusClaims
        virtual int GetMaxExecuteCounter();
        virtual void OnCreation();
        virtual void OnDestruction();
        virtual void OnInitialization();
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
        virtual bool IsDTSAgent(); -> used in GroundingManagerAgent Run's method
        virtual bool IsConversationSynchronous();
        virtual bool RequiresFloor(); -> DMCore thread diagram
        virtual void Undo();
        virtual CConcept& C(string sConceptPath);
        virtual CConcept& C(const char* lpszConceptPath, ...);
        virtual CConcept& LocalC(string sConceptName);
        CDialogAgent& A(string sDialogAgentPath);
        CDialogAgent& A(const char* lpszDialogAgentPath, ...);
        void AddSubAgent(CDialogAgent* pdaWho, CDialogAgent* pdaWhere, TAddSubAgentMethod asamMethod);
        void DeleteSubAgent(CDialogAgent* pdaWho);
        void DeleteDynamicSubAgents();
        void SetParent(CDialogAgent* pdaAParent); -> CreateDialogTree diagram 
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
        void IncrementTurnsInFocusCounter(); -> used by AcquireNextEvent
        int GetTurnsInFocusCounter(); -> used by Concept's Update_NPU_UpdateWithConcept
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
        virtual CConcept* EmptyClone(); -> used in performConceptBinding
        virtual void Update(string sUpdateType, void* pUpdateData); -> used in performConceptBinding
        virtual void Update_NPU_AssignFromString(void* pUpdateData); -> used in performConceptBinding
        virtual void Update_NPU_AssignFromConcept(void* pUpdateData); -> used with the assignment operator
        virtual void Update_NPU_UpdateWithConcept(void* pUpdateData); -> used in performConceptBinding
        virtual void Update_NPU_CollapseToMode(void* pUpdateData); -> used in performConceptBinding
        virtual void Update_PartialFromString(void* pUpdateData); -> used in performConceptBinding
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
        virtual bool IsUndergoingGrounding(); -> used in performConceptBinding
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
        virtual void SetOwnerDialogAgent(CDialogAgent* pADialogAgent);
        CDialogAgent* GetOwnerDialogAgent();
        virtual void SetOwnerConcept(CConcept* pAConcept);
        CConcept* GetOwnerConcept();
        virtual void CreateGroundingModel(string sGroundingModelSpec); -> CreateDialogTree diagram
        CGMConcept* GetGroundingModel(); -> Used by GroundingManagerAgent and DMCoreAgent
        virtual void SetGroundedFlag(bool bAGrounded); -> used by NotifyChange
        virtual bool GetGroundedFlag();
        virtual void DeclareGroundingModels(TGroundingModelPointersVector& rgmpvModels, TGroundingModelPointersSet& rgmpsExclude);
        virtual void DeclareConcepts(TConceptPointersVector& rcpvConcepts, TConceptPointersSet& rcpsExclude);
        virtual void SetInvalidatedFlag(bool bAInvalidated); -> used by NotifyChange
        virtual bool GetInvalidatedFlag();
        virtual void SetRestoredForGroundingFlag(bool bARestoredForGrounding);
        virtual bool GetRestoredForGroundingFlag();
        virtual void Seal(); -> used only by GroundingManagerAgent in Run, DMCoreAgent in performForcedConceptUpdates and Concept in Update_Calista_UpdateWithConcept
        virtual void BreakSeal(); -> used in Concept's SetInvalidatedFlag and NotifyChange
        virtual bool IsSealed(); -> used in performeForcedConceptUpdates,  and Concept
        virtual void DisableChangeNotification();
        virtual void EnableChangeNotification();
        virtual void SetChangeNotification(bool bAChangeNotification);
        virtual void NotifyChange(); -> used in performConceptBinding
        virtual CHyp* HypFactory(); -> used in Update
        virtual int AddHyp(CHyp* pAHyp);
        virtual int AddNewHyp(); -> used in Update
        virtual int AddNullHyp();
        virtual void SetHyp(int iIndex, CHyp* pHyp);
        virtual void SetNullHyp(int iIndex);
        virtual void DeleteHyp(int iIndex);
        virtual CHyp* GetHyp(int iIndex);
        virtual int GetHypIndex(CHyp* pHyp);
        virtual float GetHypConfidence(int iIndex);
        virtual void SetHypConfidence(int iIndex, float fConfidence);
        virtual CHyp* GetTopHyp(); -> used in performConceptUpdates
        virtual int GetTopHypIndex();
        virtual int Get2ndHypIndex();
        virtual float GetTopHypConfidence();
        virtual bool IsValidHyp(int iIndex);
        virtual int GetNumHyps();
        virtual int GetNumValidHyps();
        virtual void ClearCurrentHypSet(); -> used in performConceptBinding and nearly all subclasses
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
        virtual int AddNewPartialHyp(); -> used in Update_PartialFromString
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
        virtual void SetTurnLastUpdated(int iTurn); -> used in NotifyChange
        virtual void MarkTurnLastUpdated(); -> used in NotifyChange
        virtual int GetTurnLastUpdated();
        int GetTurnsSinceLastUpdated();
        void SetWaitingConveyance();
        void ClearWaitingConveyance(); -> used by NotifyChange
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
        TExecutionStack esExecutionStack; -> Core attribute
        CExecutionHistory ehExecutionHistory; -> Core attribute
        TBindingHistory bhBindingHistory; -> Core attribute
        TExpectationAgenda eaAgenda; -> Core attribute
        TFocusClaimsList fclFocusClaims; -> Core attribute
        TSystemAction saSystemAction; -> used in performForcedConceptUpdates
        int iTimeoutPeriod;
        int iDefaultTimeoutPeriod;
        float fNonunderstandingThreshold;
        float fDefaultNonunderstandingThreshold;
        STRING2BFF s2bffFilters;
        bool bFocusClaimsPhaseFlag; -> Used in the main loop
        bool bAgendaModifiedFlag;
        TFloorStatus fsFloorStatus; -> binded to fsFree and changed to fsUser or fsSystem in Execute; changed to any state in AcquireNextEvent
        int iTurnNumber;
        TCustomStartOverFunct csoStartOverFunct; -> used by RegisterCustomStartOver
    BEHAVIOUR:
        CDMCoreAgent(string sAName, string sAConfiguration, string sAType);
        virtual ~CDMCoreAgent();
        static CAgent* AgentFactory(string sAName, string sAConfiguration);
        virtual void Reset();
        void Execute(); -> DMCore thread diagram
        void AcquireNextEvent(); -> DMCore thread diagram
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
        void SignalFocusClaimsPhase(bool bAFocusClaimsPhaseFlag); -> AcquireNextEvent diagram
        void SetFloorStatus(TFloorStatus fsaFloorStatus); -> DMCore thread diagram, AcquireNextEvent diagram
        TFloorStatus GetFloorStatus(); -> DMCore thread diagram
        string FloorStatusToString(TFloorStatus fsAFloor);
        TFloorStatus StringToFloorStatus(string sAFloor);
        int LastTurnGetConceptsBound();
        bool LastTurnNonUnderstanding(); -> Used in Run diagram by GroundingManagerAgent
        int GetNumberNonUnderstandings();
        int GetTotalNumberNonUnderstandings();
        void ContinueWith(CAgent* paPusher, CDialogAgent* pdaDialogAgent); -> DMCoreAgent in Execute, resolveFocusShitf, RestartTopic and StartOver
        void RestartTopic(CDialogAgent* pdaDialogAgent);
        void RegisterCustomStartOver(TCustomStartOverFunct csoAStartOverFunct);
        void StartOver(); -> DMCore thread diagram
        CDialogAgent* GetAgentInFocus(); -> DMCore thread diagram and AcquireNextEvent
        CDialogAgent* GetDTSAgentInFocus();
        bool AgentIsInFocus(CDialogAgent* pdaDialogAgent);
        CDialogAgent* GetAgentPreviouslyInFocus(CDialogAgent* pdaDialogAgent);
        CDialogAgent* GetDTSAgentPreviouslyInFocus( CDialogAgent* pdaDialogAgent);
        CDialogAgent* GetCurrentMainTopicAgent();
        bool AgentIsActive(CDialogAgent* pdaDialogAgent); -> used by assembleFocusClaims
        void PopAgentFromExecutionStack(CDialogAgent* pdaADialogAgent);
        void PopTopicFromExecutionStack(CDialogAgent* pdaADialogAgent);
        void PopGroundingAgentsFromExecutionStack();
        int GetBindingHistorySize();
        const TBindingsDescr& GetBindingResult(int iBindingHistoryIndex);
        int GetLastInputTurnNumber(); -> used by Concept's NotifyChange
        TSystemActionOnConcept GetSystemActionOnConcept(CConcept* pConcept);
        void SignalExplicitConfirmOnConcept(CConcept* pConcept);
        void SignalImplicitConfirmOnConcept(CConcept* pConcept);
        void SignalUnplannedImplicitConfirmOnConcept(int iState, CConcept* pConcept);
        void SignalAcceptOnConcept(CConcept* pConcept);
        int popCompletedFromExecutionStack(); -> DMCore thread diagram
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
        void assembleExpectationAgenda(); -> called in UpdateState
        void compileExpectationAgenda(); -> called in UpdateState
        void enforceBindingPolicies(); -> used in assembleExpectationAgenda
        void broadcastExpectationAgenda();
        string expectationAgendaToString();
        string expectationAgendaToBroadcastString();
        string expectationAgendaToBroadcastString(TExpectationAgenda eaBAgenda);
        string bindingsDescrToString(TBindingsDescr& rbdBindings);
        void updateInputLineConfiguration();
        int assembleFocusClaims(); -> used in DMCore thread diagram
        void bindConcepts(TBindingsDescr& rbdBindings); -> Used in AcquireNextEvent
        void performConceptBinding(string sSlotName, string sSlotValue, float fConfidence, int iExpectationIndex, bool bIsComplete); -> used in AcquireNextEvent
        void performCustomConceptBinding(int iExpectationIndex);
        void performForcedConceptUpdates(TBindingsDescr& rbdBindings); -> used in AcquireNextEvent
        void processNonUnderstanding();
        void resolveFocusShift(); -> DMCore thread diagram
        void rollBackDialogState(int iState);

file: Agents/CoreAgents/DTTManagerAgent.h
    class CDTTManagerAgent: public CAgent
    DATA:
        CDialogAgent* pdaDialogTaskRoot; -> Used by DTTManagerAgent and DMCoreAgent
        vector<TDiscourseAgentInfo, allocator<TDiscourseAgentInfo>> vdaiDAInfo; -> CreateDialogTree diagram
    BEHAVIOUR:
        CDTTManagerAgent(string sAName, string sAConfiguration, string sAType);
        virtual ~CDTTManagerAgent();
        static CAgent* AgentFactory(string sAName, string sAConfiguration);
        virtual void Reset();
        void Use(string sDAType, string sDAName, FRegisterAgent fRegisterAgent, string sConfiguration); -> DMCore thread diagram
        void CreateDialogTree(); -> DMCore thread diagram
        void DestroyDialogTree(); -> used by StartOver
        void ReCreateDialogTree(); -> used by StartOver
        void CreateDialogTaskTree();
        void CreateDialogTaskAgentome();
        CDialogAgent* GetDialogTaskTreeRoot(); -> Used only by DMCoreAgent
        void MountAgent(CDialogAgent* pdaWhere, CDialogAgent* pdaWho, TMountingMethod mmHow, string sDynamicAgentID); -> CreateDialogTree diagram
        CDialogAgent* MountAgent(CDialogAgent* pWhere, string sAgentType, string sAgentName, string sAConfiguration, TMountingMethod mmHow, string sDynamicAgentID); -> CreateDialogTree diagram
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
        TGroundingManagerConfiguration gmcConfig; -> Used in CreateDialogTree diagram by the root DialogAgency
        bool bTurnGroundingRequest; -> Run diagram
        TConceptGroundingRequests vcgrConceptGroundingRequests; -> Used in Run
        bool bLockedGroundingRequests; -> Used by ScheduleConceptGrounding and Run
        vector<TGroundingActionHistoryItems> vgahiGroundingActionsHistory; -> Used inside this class
        TGroundingModelsTypeHash gmthGroundingModelTypeRegistry;
    BEHAVIOUR:
        CGroundingManagerAgent(string sAName, string sAConfiguration, string sAType);
        virtual ~CGroundingManagerAgent();
        static CAgent* AgentFactory(string sAName, string sAConfiguration);
        void SetConfiguration(string sAGroundingManagerConfiguration);
        TGroundingManagerConfiguration GetConfiguration(); -> CreateDialogTree diagram
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
        CGroundingAction* operator[] (unsigned int iGroundingActionIndex); -> used by GroundingModel in RunAction
        void RequestTurnGrounding(bool bATurnGroundingRequest); -> used by AcquireNextEvent
        void RequestConceptGrounding(CConcept* pConcept); -> used by Concept's NotifyChange
        string ScheduleConceptGrounding(CConcept* pConcept); -> used by GAExplicitConfirm, Update_Calista_UpdateWithConcept and performForcedConceptUpdates
        void LockConceptGroundingRequestsQueue(); -> Used by ScheduleConceptGrounding and Run
        void UnlockConceptGroundingRequestsQueue(); -> Used by ScheduleConceptGrounding and Run
        void SetConceptGroundingRequestStatus(CConcept* pConcept, int iAGroundingRequestStatus);
        int GetConceptGroundingRequestStatus(CConcept* pConcept); -> used by performForcedConceptUpdates
        void ConceptGroundingRequestCompleted(CConcept* pConcept);
        void RemoveConceptGroundingRequest(CConcept* pConcept); -> used in performConceptBinding
        void PurgeConceptGroundingRequestsQueue(); -> Used in Run diagram
        void GAHAddHistoryItem(string sGroundingModelName, string sActionName, int iGroundingActionType); -> used in GroundingManagerAgent's Run
        void GAHSetBargeInFlag(int iTurnNum, bool bBargeInFlag);
        string GAHGetTurnGroundingAction(int iTurnNumber);
        int GAHCountTakenInLastNTurns(bool bAlsoHeard, string sActionName, int iNumTurns);
        int GAHCountTakenByGroundingModelInLastNTurns(bool bAlsoHeard, string sActionName, string sGroundingModelName, int iNumTurns);
        void RegisterGroundingModelType(string sName, FCreateGroundingModel fctCreateGroundingModel);
        CGroundingModel* CreateGroundingModel(string sModelType, string sModelPolicy); -> CreateDialogTree diagram
        bool HasPendingRequests(); -> DMCore thread diagram
        bool HasPendingTurnGroundingRequest();
        bool HasPendingConceptGroundingRequests();
        bool HasUnprocessedConceptGroundingRequests(); -> DMCore thread diagram
        bool HasScheduledConceptGroundingRequests(); -> DMCore thread diagram and assembleFocusClaims
        bool HasExecutingConceptGroundingRequests();
        bool GroundingInProgressOnConcept(CConcept* pConcept);
        string GetScheduledGroundingActionOnConcept(CConcept* pConcept); -> used in performForcedConcepUpdates
        virtual void Run(); -> DMCore thread diagram
        int getConceptGroundingRequestIndex(CConcept* pConcept);
        string loadPolicy(string sFileName);

file: Grounding/GroundingModels/GroundingModel.h
    class CGroundingModel
    DATA:
        string sName;
        string sModelPolicy;
        TPolicy pPolicy; -> Used in ComputeActionValuesDistribution
        bool bExternalPolicy; -> Used in ComputeSuggestedActionIndex
        CExternalPolicyInterface* pepiExternalPolicy;
        string sExternalPolicyHost;
        vector<int> viActionMappings; -> used in subclasses and ComputeActionValuesDistribution
        string sExplorationMode;
        float fExplorationParameter;
        CState stFullState;
        CBeliefDistribution bdBeliefState; -> used in subclasses
        CBeliefDistribution bdActionValues; -> used in subclasses and ComputeActionValuesDistribution
        int iSuggestedActionIndex; -> result of the ComputeSuggestedActionIndex
    BEHAVIOUR:
        CGroundingModel(string sAModelPolicy, string sAModelName);
        CGroundingModel(CGroundingModel& rAGroundingModel);
        virtual ~CGroundingModel();
        static CGroundingModel* GroundingModelFactory(string sModelPolicy); -> CreateDialogTree diagram
        virtual string GetType();
        virtual string GetModelPolicy();
        virtual string GetName();
        virtual void SetName(string sAName);
        virtual void Initialize(); -> CreateDialogTree diagram
        virtual CGroundingModel* Clone() = 0;
        virtual bool LoadPolicy();
        virtual void ComputeState(); -> used in Run diagram by GroundingManagerAgent
        virtual void ComputeActionValuesDistribution(); -> can be defined in subclasses, used in ComputeSuggestedActionIndex
        virtual int ComputeSuggestedActionIndex(); -> can be defined in subclasses, used by the GroundingManagerAgent in Run
        virtual void Run();
        virtual void RunAction(int iActionIndex); Used by GroundingManagerAgent when the iActinIndex is computed and an action needs to be run
        virtual void LogStateAction(); -> used in GroundingManagerAgent
        virtual void computeFullState() = 0; -> defined in subclasses, used by ComputeState
        virtual void computeBeliefState() = 0; -> defined in subclasses, used by ComputeState
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
        virtual void Run(void *pParams) = 0; -> Used by the GroundingModel in RunAction
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
        void BroadcastState(); used by AcquireNextEvent
        void UpdateState(); -> DMCore thread, AcquireNextEvent diagrams and StartOver
        string GetStateAsString(TDialogState dsState);
        string GetStateAsString();
        int GetStateHistoryLength();
        TDialogState &GetLastState(); -> Run diagram
        TDialogState &operator[](unsigned int i);

file: Agents/CoreAgents/InteractionEventManagerAgent.h
    class CInteractionEventManagerAgent: public CAgent
    DATA:
        HANDLE hNewInteractionEvent; -> Core attribute
        queue <CInteractionEvent*, list<CInteractionEvent*>> qpieEventQueue; -> Core attribute
        vector <CInteractionEvent*> vpieEventHistory; -> Core attribute
        CInteractionEvent *pieLastEvent; -> Core attribute
        CInteractionEvent *pieLastInput; -> Core attribute
    BEHAVIOUR:
        CInteractionEventManagerAgent(string sAName, string sAConfiguration, string sAType);
        virtual ~CInteractionEventManagerAgent();
        static CAgent* AgentFactory(string sAName, string sAConfiguration);
        virtual void Reset();
        virtual void Initialize();
        bool HasEvent();
        CInteractionEvent *GetNextEvent(); -> used in AcquireNextEvent DMCoreAgent
        CInteractionEvent *GetLastEvent(); -> used in bindConcepts' DMCoreAgent
        CInteractionEvent *GetLastInput(); -> used by many Core Agents
        bool LastEventMatches(string sGrammarExpectation); -> Used in bindConcepts' DMCoreAgent
        bool LastInputMatches(string sGrammarExpectation);
        bool LastEventIsComplete(); -> Used in bindConcepts' DMCoreAgent
        float GetLastEventConfidence(); -> Used in bindConcepts' DMCoreAgent
        string GetValueForExpectation(string sGrammarExpectation); -> used by many Core Agents
        void WaitForEvent(); -> Used in AcquireNextEvent diagram
        void SignalInteractionEventArrived(); -> DMBridge thread

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
        void Notify(int iOutputId, int iBargeinPos, string sConveyance, string sTaggedUtt); -> used by AcquireNextEvent
        void PreliminaryNotify(int iOutputId, string sTaggedUtt); -> used by AcquireNextEvent
        void CancelConceptNotificationRequest(CConcept* pConcept); -> used by Concept's ClearWaitingConveyance
        void ChangeConceptNotificationPointer(CConcept* pOldConcept, CConcept* pNewConcept);
        string GetPromptsWaitingForNotification(); -> DMCore thread diagram
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
