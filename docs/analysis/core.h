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
        bool IsRegisteredAgent(string sAgentName); -> Used only as an internal method
        CAgent* operator[](string sAgentName); -> Used by DMCoreAgent, GroundingManagerAgent, Concept and Outputs
        void RegisterAgentType(string sAgentTypeName, FCreateAgent fctCreateAgent); -> CreateDialogTree diagram
        void UnRegisterAgentType(string sAgentTypeName); -> Used in the Agent's destructor
        bool IsRegisteredAgentType(string sAgentType);
        CAgent* CreateAgent(string sAgentTypeName, string sAgentName, string sAgentConfiguration); -> CreateDialogTree diagram

file: Agents/Agent.h
    class CAgent
    DATA:
        string sName;
        tring sType;
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
        virtual void UnRegister(); -> Used in the destructor
        virtual void Create();
        virtual void Initialize();
        virtual void Reset();

file: Agents/DialogAgents/DialogAgent.h
    class CDialogAgent : public CAgent
    DATA:
        string sDialogAgentName;
        TConceptPointersVector Concepts; -> Core attribute
        TAgentsVector SubAgents;
        CDialogAgent* pdaParent;
        CDialogAgent* pdaContextAgent; -> Semicore attribute
        CGroundingModel* pGroundingModel; -> Core attribute
        bool bCompleted; -> Core attribute
        TCompletionType ctCompletionType; -> Core attribute
        bool bBlocked;
        bool bDynamicAgent;
        string sDynamicAgentID;
        string sTriggeredByCommands; -> Core attribute
        string sTriggerCommandsGroundingModelSpec; -> Core attribute, this is the GroundingModel spec for the added concept
        int iExecuteCounter; -> semicore attribute
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
        CDialogAgent(string sAName, string sAConfiguration, string sAType); -> Core abstraction
        virtual ~CDialogAgent(); -> Core abstraction
        static CAgent* AgentFactory(string sAName, string sAConfiguration); -> CreateDialogTree diagram
        virtual void Register(); -> CreateDialogTree diagram
        virtual void Create(); -> CreateDialogTree diagram
        virtual void Initialize() -> CreateDialogTree diagram;
        virtual void Reset(); -> Core
        virtual void CreateConcepts(); -> CreateDialogTree diagram
        virtual bool IsExecutable(); -> Core in GetAgentInFocus to avoid using some Agents like MAExpect
        virtual TDialogExecuteReturnCode Execute(); -> DMCore thread diagram
        virtual int DeclareExpectations(TConceptExpectationList& rcelExpectationList); -> called in compileExpectationAgenda
        virtual void DeclareConcepts(TConceptPointersVector& rcpvConcepts, TConceptPointersSet& rcpsExclude);
        virtual void CreateGroundingModel(string sGroundingModelSpec);
        virtual CGroundingModel* GetGroundingModel(); -> Used by GroundingManagerAgent
        virtual void DeclareGroundingModels(TGroundingModelPointersVector& rgmpvModels, TGroundingModelPointersSet& rgmpsExclude);
        virtual bool ExpectCondition();
        virtual string DeclareBindingPolicy(); -> this method belongs to the ExpectationAgenda
        virtual int DeclareFocusClaims(TFocusClaimsList& fclFocusClaims); -> used by DMCoreAgent's assembleFocusClaims
        virtual bool PreconditionsSatisfied();
        virtual bool ClaimsFocus(); -> used in DialogAgent's DeclareFocusClaims
        virtual bool ClaimsFocusDuringGrounding(); -> used in DialogAgent's DeclareFocusClaims
        virtual string TriggeredByCommands(); -> Core abstraction
        void CreateTriggerConcept(); -> used by Register function when the CreateDialogTree is called
        virtual bool SuccessCriteriaSatisfied(); -> defined by subclasses, used by assembleFocusClaims and HasSucceeded
        virtual bool FailureCriteriaSatisfied(); -> defined by subclasses, used by assembleFocusClaims and HasFailed
        virtual int GetMaxExecuteCounter(); -> semicore, has dependencies throughout the core
        virtual void OnCreation();
        virtual void OnDestruction();
        virtual void OnInitialization();
        virtual void OnCompletion();
        virtual void ReOpen(); -> can be overwritten by the user, called in RestartTopic (which no one uses)
        virtual void ReOpenConcepts(); -> is called by ReOpen
        virtual void ReOpenTopic(); -> is called by ReOpen
        virtual bool IsAMainTopic(); -> used by DMCoreAgent's GetCurrentMainTopic
        virtual string Prompt();
        virtual string TimeoutPrompt();
        virtual string ExplainMorePrompt();
        virtual string CreateVersionedPrompt(string sVersion);
        virtual string EstablishContextPrompt();
        virtual string WhatCanISayPrompt();
        virtual string InputLineConfigurationInitString();
        virtual bool IsDTSAgent(); -> used in GroundingManagerAgent Run's method and DMCoreAgent in its functions related to the execution stack
        virtual bool IsConversationSynchronous();
        virtual bool RequiresFloor(); -> Used by DMCore's Execute
        virtual void Undo();
        virtual CConcept& C(string sConceptPath); -> Query published interface
        virtual CConcept& C(const char* lpszConceptPath, ...); -> Query published interface
        virtual CConcept& LocalC(string sConceptName); -> Query published interface
        CDialogAgent& A(string sDialogAgentPath); -> Query published interface
        CDialogAgent& A(const char* lpszDialogAgentPath, ...); -> Query published interface
        void AddSubAgent(CDialogAgent* pdaWho, CDialogAgent* pdaWhere, TAddSubAgentMethod asamMethod); -> used by DTT's Mount family of methods
        void DeleteSubAgent(CDialogAgent* pdaWho); -> used by DTT's UnMount family of methods
        void DeleteDynamicSubAgents();
        void SetParent(CDialogAgent* pdaAParent); -> used by DTT's CreateDialogTree
        CDialogAgent* GetParent(); -> used by DTT
        void UpdateName(); -> used by SetParent to update the hierarchichal name
        void SetContextAgent(CDialogAgent* pdaAContextAgent);
        CDialogAgent* GetContextAgent();
        CDialogAgent* GetMainTopic();
        bool HasCompleted(); -> used by DMCoreAgent's popCompletedFromExecutionStack
        void SetCompleted(TCompletionType ctACompletionType = ctSuccess); -> Core abstraction
        void ResetCompleted();
        bool HasFailed(); -> used in HasCompleted
        bool HasSucceeded(); -> used in HasCompleted
        bool IsAgentPathBlocked();
        virtual bool IsBlocked();
        void Block();
        void UnBlock();
        void SetDynamicAgent();
        bool IsDynamicAgent();
        void SetDynamicAgentID(string sADynamicAgentID);
        string GetDynamicAgentID();
        void IncrementExecuteCounter(); -> semicore
        int GetExecuteCounter(); -> semicore
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
        void parseGrammarMapping(string sConceptNames, string sGrammarMapping, TConceptExpectationList& rcelExpectationList); -> used by DeclareExpectations

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
        TConceptType ctConceptType; -> Core attribute
        TConceptSource csConceptSource;
        string sName; -> Core attribute
        CDialogAgent* pOwnerDialogAgent; -> Core attribute?
        CConcept* pOwnerConcept; -> used by IsGrounded
        CGMConcept* pGroundingModel; -> Core attribute
        bool bGrounded; -> used by IsUpdatedAndGrounded
        bool bInvalidated;
        bool bRestoredForGrounding; -> used by SetGroundedFlag
        bool bSealed; -> Core attribute, used in the grounding process
        bool bChangeNotification; -> used by SetGroundedFlag
        vector<CHyp*, allocator<CHyp*>> vhCurrentHypSet; -> Core attribute
        int iNumValidHyps;
        vector<CHyp*, allocator<CHyp*>> vhPartialHypSet; -> Used moslty by StructConcept
        int iNumValidPartialHyps; -> Used moslty by StructConcept
        int iCardinality; -> Used inside
        int iTurnLastUpdated; -> Core? is only used by some subclasses
        TConveyance cConveyance; -> Core attribute
        bool bWaitingConveyance;
        CConcept* pPrevConcept; -> Core attribute
        bool bHistoryConcept; -> Core attribute
        string sExplicitlyConfirmedHyp; -> Only used by the update process
        string sExplicitlyDisconfirmedHyp; -> Only used by the update process
    BEHAVIOUR:
        CConcept(string sAName, TConceptSource csAConceptSource, int iACardinality); -> Core behaviour
        virtual ~CConcept(); -> Core behaviour
        virtual CConcept& operator = (CConcept& rAConcept); -> Core behaviour
        virtual CConcept& operator = (int iAValue); -> Core behaviour
        virtual CConcept& operator = (bool bAValue); -> Core behaviour
        virtual CConcept& operator = (float fAValue); -> Core behaviour
        virtual CConcept& operator = (double dAValue); -> Core behaviour
        virtual CConcept& operator = (string sAValue); -> Core behaviour
        virtual CConcept& operator = (const char* lpszAValue); -> Core behaviour
        virtual bool operator == (CConcept& rAConcept); -> Core behaviour
        virtual bool operator != (CConcept& rAConcept); -> Core behaviour
        virtual bool operator < (CConcept& rAConcept); -> Core behaviour
        virtual bool operator > (CConcept& rAConcept); -> Core behaviour
        virtual bool operator <= (CConcept& rAConcept); -> Core behaviour
        virtual bool operator >= (CConcept& rAConcept); -> Core behaviour
        virtual CConcept& operator [](int iIndex); -> Core behaviour, used by GroundingManager.PrecomputeBeliefUpdatingFeatures
        virtual CConcept& operator [](string sIndex); -> Core behaviour
        virtual CConcept& operator [](const char* lpszIndex); -> Core behaviour
        virtual operator int(); -> Core behaviour
        virtual operator float(); -> Core behaviour
        virtual operator bool(); -> Core behaviour
        virtual operator string(); -> Core behaviour
        virtual void Clear(); -> used by DialogAgent in Reset
        virtual void ClearCurrentValue();
        virtual CConcept* Clone(bool bCloneHistory);
        virtual CConcept* EmptyClone(); -> used in performConceptBinding
        virtual void Update(string sUpdateType, void* pUpdateData); -> used in performConceptBinding
        virtual void Update_NPU_AssignFromString(void* pUpdateData); -> used in performConceptBinding
        virtual void Update_NPU_AssignFromConcept(void* pUpdateData); -> used with the assignment operator
        virtual void Update_NPU_UpdateWithConcept(void* pUpdateData); -> used in performConceptBinding
        virtual void Update_NPU_CollapseToMode(void* pUpdateData); -> used in performConceptBinding
        virtual void Update_PartialFromString(void* pUpdateData); -> used in performConceptBinding and by StructConcept
        virtual void Update_Calista_AssignFromString(void* pUpdateData); -> used in performConceptBinding
        virtual void Update_Calista_AssignFromConcept(void* pUpdateData); -> used in performConceptBinding
        virtual void Update_Calista_UpdateWithConcept(void* pUpdateData); -> used in performConceptBinding
        virtual void Update_Calista_CollapseToMode(void* pUpdateData); -> used in performConceptBinding
        virtual bool IsUpdated(); -> used inside the class
        virtual bool IsUpdatedAndGrounded(); ->  used inside and by DialogAgent
        virtual bool IsAvailable(); -> used by IsAvailableAndGrounded
        virtual bool IsAvailableAndGrounded(); -> used by Calista
        virtual bool HasPartialUpdate();
        virtual bool IsGrounded();
        virtual bool IsUndergoingGrounding(); -> used in performConceptBinding
        virtual bool IsInvalidated();
        virtual bool IsRestoredForGrounding();
        virtual string GroundedHypToString();
        virtual string TopHypToString();
        virtual string HypSetToString();
        virtual TConceptType GetConceptType(); -> Core
        virtual void SetConceptType(TConceptType ctAConceptType); -> Core
        virtual TConceptSource GetConceptSource();
        virtual void SetConceptSource(TConceptSource csAConceptSource);
        virtual void SetName(string sAName); -> Core
        string GetName(); -> Core
        string GetSmallName(); -> Used by GroundingManagerAgent's PrecomputeBeliefUpdatingFeatures
        string GetAgentQualifiedName(); -> Used inside and by GroundingManagerAgent and DMCoreAgent
        virtual void SetOwnerDialogAgent(CDialogAgent* pADialogAgent);
        CDialogAgent* GetOwnerDialogAgent();
        virtual void SetOwnerConcept(CConcept* pAConcept);
        CConcept* GetOwnerConcept();
        virtual void CreateGroundingModel(string sGroundingModelSpec); -> used in DTT's CreateDialogTree; as in the DialogAgent method, a Concept shouldn't be responsible of the creation of grounding models
        CGMConcept* GetGroundingModel(); -> Used by GroundingManagerAgent and DMCoreAgent
        virtual void SetGroundedFlag(bool bAGrounded); -> used by NotifyChange, the bGrounded flag is only useful in the IsUpdatedAndGrounded; this method is too much complex.
        virtual bool GetGroundedFlag(); Used in MergeHistory
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
        virtual int AddHyp(CHyp* pAHyp); -> Core behaviour
        virtual int AddNewHyp(); -> Core behaviour
        virtual int AddNullHyp();
        virtual void SetHyp(int iIndex, CHyp* pHyp); -> Core behaviour
        virtual void SetNullHyp(int iIndex);
        virtual void DeleteHyp(int iIndex); -> Core behaviour
        virtual CHyp* GetHyp(int iIndex); -> Core behaviour, used by GroundingManager
        virtual int GetHypIndex(CHyp* pHyp);
        virtual float GetHypConfidence(int iIndex);
        virtual void SetHypConfidence(int iIndex, float fConfidence); -> Core, used inside
        virtual CHyp* GetTopHyp(); -> used inside, in subclasses and in performConceptUpdates
        virtual int GetTopHypIndex(); -> used inside and in PrecomputeBeliefUpdatingFeatures
        virtual int Get2ndHypIndex();
        virtual float GetTopHypConfidence(); -> used only by GMConcept
        virtual bool IsValidHyp(int iIndex);
        virtual int GetNumHyps(); -> core, used inside
        virtual int GetNumValidHyps(); -> is only used inside the normal Update to set the invalidated flag
        virtual void ClearCurrentHypSet(); -> used in performConceptBinding and nearly all subclasses
        virtual void CopyCurrentHypSetFrom(CConcept& rAConcept); -> Core, used in Clone, Update and SetGroundedFlag
        virtual void SetCardinality(int iACardinality);
        virtual int GetCardinality();
        virtual float GetPriorForHyp(CHyp* pHyp); -> used by GroundingManager.PrecomputeBeliefUpdatingFeatures
        virtual float GetConfusabilityForHyp(CHyp* pHyp); -> used by GroundingManager.PrecomputeBeliefUpdatingFeatures
        virtual string GetConceptTypeInfo(); -> this is not the ctConceptType attribute, it's an abstraction implemented in the GroundingManager that is only used by the GroundingManager
        virtual void SetExplicitlyConfirmedHyp(CHyp* pHyp); -> Used in the updating process
        virtual void SetExplicitlyConfirmedHyp(string sAExplicitlyConfirmedHyp);
        virtual void SetExplicitlyDisconfirmedHyp(CHyp* pHyp);
        virtual void SetExplicitlyDisconfirmedHyp(string sAExplicitlyDisconfirmedHyp); -> Used in the updating process
        virtual string GetExplicitlyConfirmedHypAsString(); -> Used in CopyCurrentHypSetFrom
        virtual string GetExplicitlyDisconfirmedHypAsString(); -> Used in CopyCurrentHypSetFrom
        virtual void ClearExplicitlyConfirmedHyp();
        virtual void ClearExplicitlyDisconfirmedHyp();
        virtual int AddPartialHyp(CHyp* pAHyp); -> used only by StructConcept
        virtual int AddNewPartialHyp(); -> used in Update_PartialFromString
        virtual int AddNullPartialHyp(); -> used only by StructConcept
        virtual bool HasPartialHyp(); -> used only by StructConcept
        virtual CHyp* GetPartialHyp(int iIndex); -> used only by StructConcept
        virtual CHyp* GetTopPartialHyp();
        virtual int GetTopPartialHypIndex();
        virtual float GetTopPartialHypConfidence();
        bool IsValidPartialHyp(int iIndex);
        virtual int GetNumPartialHyps(); -> used only by StructConcept
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
        virtual void ClearConceptNotificationPointer(); -> used by subclasses
        virtual void ReOpen(); -> called by ReOpenConcept in DialogAgent
        virtual void Restore(int iIndex);
        virtual void ClearHistory(); -> used by the normal update process, Restore and MergeHistory
        virtual CConcept* CreateMergedHistoryConcept();
        virtual void MergeHistory();
        int GetHistorySize(); -> used in GroundingManager.PrecomputeBeliefUpdatingFeatures
        virtual CConcept& GetHistoryVersion(int iIndex);
        virtual void SetHistoryConcept(bool bAHistoryConcept); -> used by subclasses and inside
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
        float fNonunderstandingThreshold; -> Core, used in bindConcepts
        float fDefaultNonunderstandingThreshold;
        STRING2BFF s2bffFilters; -> Core
        bool bFocusClaimsPhaseFlag; -> Used in the main loop
        bool bAgendaModifiedFlag; -> Used by StateManager.UpdateState
        TFloorStatus fsFloorStatus; -> binded to fsFree and changed to fsUser or fsSystem in Execute; changed to any state in AcquireNextEvent
        int iTurnNumber; -> Core attribute, used by GroundingManager, DMCoreAgent, StateManager
        TCustomStartOverFunct csoStartOverFunct; -> used by RegisterCustomStartOver
    BEHAVIOUR:
        CDMCoreAgent(string sAName, string sAConfiguration, string sAType); -> Core
        virtual ~CDMCoreAgent(); -> Core
        static CAgent* AgentFactory(string sAName, string sAConfiguration); -> Core
        virtual void Reset(); -> Core
        void Execute(); -> DMCore thread diagram
        void AcquireNextEvent(); -> DMCore thread diagram
        void RegisterBindingFilter(string sBindingFilterName, TBindingFilterFunct bffFilter);
        int GetIntSessionID();
        void SetTimeoutPeriod(int iATimeoutPeriod); -> used in MARequest
        int GetTimeoutPeriod();
        void SetDefaultTimeoutPeriod(int iADefaultTimeoutPeriod);
        int GetDefaultTimeoutPeriod();
        void SetNonunderstandingThreshold(float fANonunderstandingThreshold); -> Used by MARequest
        float GetNonunderstandingThreshold(); -> Used in StateManager
        void SetDefaultNonunderstandingThreshold(float fANonuThreshold);
        float GetDefaultNonunderstandingThreshold();
        void SignalFocusClaimsPhase(bool bAFocusClaimsPhaseFlag); -> AcquireNextEvent diagram
        void SetFloorStatus(TFloorStatus fsaFloorStatus); -> DMCore thread diagram, AcquireNextEvent diagram
        TFloorStatus GetFloorStatus(); -> DMCore thread diagram
        string FloorStatusToString(TFloorStatus fsAFloor); -> used in Output
        TFloorStatus StringToFloorStatus(string sAFloor); -> used in RequireNextEvent

        // API of History
        int LastTurnGetConceptsBound();
        bool LastTurnNonUnderstanding(); -> Used in Run diagram by GroundingManagerAgent
        int GetNumberNonUnderstandings(); -> used in some GMRequests as part of the full state
        int GetTotalNumberNonUnderstandings();
        int GetBindingHistorySize();
        const TBindingsDescr& GetBindingResult(int iBindingHistoryIndex);
        int GetLastInputTurnNumber(); -> used by Concept's NotifyChange and GroundingManager

        // API of ExecutionStack
        void ContinueWith(CAgent* paPusher, CDialogAgent* pdaDialogAgent); -> DMCoreAgent in Execute, resolveFocusShitf, RestartTopic and StartOver
        void RestartTopic(CDialogAgent* pdaDialogAgent); -> Core but no one is using it
        void RegisterCustomStartOver(TCustomStartOverFunct csoAStartOverFunct);
        void StartOver(); -> DMCore thread diagram
        CDialogAgent* GetAgentInFocus(); -> DMCore thread diagram, used by AcquireNextEvent, GroundingManager and StateManager
        CDialogAgent* GetDTSAgentInFocus(); -> used by DialogAgent.parseGrammarMapping
        bool AgentIsInFocus(CDialogAgent* pdaDialogAgent); -> used by DialogAgent.parseGrammarMapping
        CDialogAgent* GetAgentPreviouslyInFocus(CDialogAgent* pdaDialogAgent);
        CDialogAgent* GetDTSAgentPreviouslyInFocus( CDialogAgent* pdaDialogAgent);
        CDialogAgent* GetCurrentMainTopicAgent(); -> used by DialogAgent's parseGrammarMapping
        bool AgentIsActive(CDialogAgent* pdaDialogAgent); -> used by assembleFocusClaims
        void PopAgentFromExecutionStack(CDialogAgent* pdaADialogAgent); -> Core
        void PopTopicFromExecutionStack(CDialogAgent* pdaADialogAgent); -> Core
        void PopGroundingAgentsFromExecutionStack();

        // API of System Actions
        TSystemActionOnConcept GetSystemActionOnConcept(CConcept* pConcept);
        void SignalExplicitConfirmOnConcept(CConcept* pConcept);
        void SignalImplicitConfirmOnConcept(CConcept* pConcept);
        void SignalUnplannedImplicitConfirmOnConcept(int iState, CConcept* pConcept);
        void SignalAcceptOnConcept(CConcept* pConcept);

        // Private API
        int popCompletedFromExecutionStack(); -> implementation side
        void popAgentFromExecutionStack(CDialogAgent *pdaADialogAgent, TStringVector& rvsAgentsEliminated); -> implementation side
        void popTopicFromExecutionStack(CDialogAgent *pdaADialogAgent, TStringVector& rvsAgentsEliminated); -> implementation side
        void popGroundingAgentsFromExecutionStack(TStringVector& rvsAgentsEliminated); -> implementation side
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
        void enforceBindingPolicies(); -> used in assembleExpectationAgenda, this method belongs to the expectation agenda
        void broadcastExpectationAgenda();
        string expectationAgendaToString();
        string expectationAgendaToBroadcastString();
        string expectationAgendaToBroadcastString(TExpectationAgenda eaBAgenda);
        string bindingsDescrToString(TBindingsDescr& rbdBindings); -> used to log in bindConcepts
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
        CDTTManagerAgent(string sAName, string sAConfiguration, string sAType); -> Core
        virtual ~CDTTManagerAgent(); -> Core
        static CAgent* AgentFactory(string sAName, string sAConfiguration); -> Core
        virtual void Reset(); -> Core
        void Use(string sDAType, string sDAName, FRegisterAgent fRegisterAgent, string sConfiguration); -> DMCore thread diagram
        void CreateDialogTree(); -> Used in DMCoreAgent's Execute and StartOver
        void DestroyDialogTree(); -> used by DMCoreAgent.StartOver
        void ReCreateDialogTree(); -> used by DMCoreAgent.StartOver
        void CreateDialogTaskTree();
        void CreateDialogTaskAgentome();
        CDialogAgent* GetDialogTaskTreeRoot(); -> Used only by DMCoreAgent's Execute, computeCurrentSystemAction, assembleFocusClaims and StartOver
        void MountAgent(CDialogAgent* pdaWhere, CDialogAgent* pdaWho, TMountingMethod mmHow, string sDynamicAgentID); -> called in (Re)CreateDialogTree
        CDialogAgent* MountAgent(CDialogAgent* pWhere, string sAgentType, string sAgentName, string sAConfiguration, TMountingMethod mmHow, string sDynamicAgentID); -> called in (Re)CreateDialogTree
        void UnMountAgent(CDialogAgent* pdaWho);
        void MountAgentsFromArrayConcept(CDialogAgent *pdaParent, string sArrayConceptName, string sAgentsType, string sAgentsName, string sAgentsConceptName, string sAgentsDynamicID); -> called in (Re)CreateDialogTree
        string GetParentName(string sAgentPath); -> used inside the class
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
        vector <CGroundingAction *> vpgaActions;
        TStringVector vsActionNames;
        TExternalPolicies mapExternalPolicies;
        TGroundingManagerConfiguration gmcConfig; -> Used in CreateDialogTree diagram by the root DialogAgency
        bool bTurnGroundingRequest; -> Run diagram
        TConceptGroundingRequests vcgrConceptGroundingRequests; -> Used in Run
        bool bLockedGroundingRequests; -> Used by ScheduleConceptGrounding and Run
        vector<TGroundingActionHistoryItems> vgahiGroundingActionsHistory; -> Used inside this class
        TGroundingModelsTypeHash gmthGroundingModelTypeRegistry;
    BEHAVIOUR:
        CGroundingManagerAgent(string sAName, string sAConfiguration, string sAType); -> Core
        virtual ~CGroundingManagerAgent(); -> Core
        static CAgent* AgentFactory(string sAName, string sAConfiguration); -> Core

        // Manage policies
        void SetConfiguration(string sAGroundingManagerConfiguration); used in DMCore.InitializeDialogCore
        TGroundingManagerConfiguration GetConfiguration(); -> used by CreateGroundingModel of DialogAgent and Concept
        virtual void LoadPoliciesFromString(string sPolicies); -> used in DMCore.InitializeDialogCore
        virtual void LoadPoliciesFromFile(string sFileName); -> used in DMCore.InitializeDialogCore
        virtual string GetPolicy(string sModelName); -> used in GroundingModels to LoadPolicy
        CExternalPolicyInterface* CreateExternalPolicyInterface(string sAHost); -> used in GroundingModels to LoadPolicy
        void ReleaseExternalPolicyInterfaces(); -> used in the destructor

        // Manage belief updating models
        virtual void LoadBeliefUpdatingModel(string sAFileName); -> called by SetBeliefUpdatingModelName
        virtual void SetBeliefUpdatingModelName(string sABeliefUpdatingModelName); -> called by SetConfiguration
        virtual string GetBeliefUpdatingModelName(); -> Used in Concept.Update
        virtual STRING2FLOATVECTOR& GetBeliefUpdatingModelForAction(string sSystemAction); -> used in calista
        virtual float GetConstantParameter(string sParam); -> used in PrecomputeBeliefUpdatingFeatures
        virtual void PrecomputeBeliefUpdatingFeatures(CConcept* pIConcept, CConcept* pNewConcept, string sSystemAction); -> used by the normal belief updating process
        virtual float GetGroundingFeature(string sFeatureName); -> used in calista
        virtual string GetGroundingFeatureAsString(string sFeatureName);
        virtual void ClearBeliefUpdatingFeatures(); -> used in calista
        virtual float GetPriorForConceptHyp(string sConcept, string sHyp);
        virtual float GetConfusabilityForConceptHyp(string sConcept, string sHyp);
        virtual string GetConceptTypeInfoForConcept(string sConcept); -> used by Concept.GetConceptTypeInfo

        // Manage vpgaActions
        void UseGroundingAction(string sActionName, GroundingAction* pgaAGroundingAction);
        int GroundingActionNameToIndex(string sGroundingActionName); -> used in GroundingModel's ComputeSuggestedActionIndex and LoadPolicy
        string GroundingActionIndexToName(unsigned int iGroundingActionIndex); -> used in GroundingModel's LogStateAction and actionValuesToString plus ScheduleConceptGrounding, GetScheduledGroundingActiononConcept and Run
        CGroundingAction* operator[] (string sGroundingActionName); -> Core
        CGroundingAction* operator[] (unsigned int iGroundingActionIndex); -> used by GroundingModel in RunAction

        // Handle requests
        void RequestTurnGrounding(bool bATurnGroundingRequest); -> used by AcquireNextEvent
        void RequestConceptGrounding(CConcept* pConcept); -> used by Concept's NotifyChange and ScheduleConceptGrounding
        string ScheduleConceptGrounding(CConcept* pConcept); -> used by GAExplicitConfirm, Update_Calista_UpdateWithConcept and performForcedConceptUpdates
        void LockConceptGroundingRequestsQueue(); -> Used by ScheduleConceptGrounding and Run
        void UnlockConceptGroundingRequestsQueue(); -> Used by ScheduleConceptGrounding and Run
        void SetConceptGroundingRequestStatus(CConcept* pConcept, int iAGroundingRequestStatus);
        int GetConceptGroundingRequestStatus(CConcept* pConcept); -> used by performForcedConceptUpdates
        void ConceptGroundingRequestCompleted(CConcept* pConcept);
        void RemoveConceptGroundingRequest(CConcept* pConcept); -> used in performConceptBinding
        void PurgeConceptGroundingRequestsQueue(); -> Used in Run diagram
        bool HasPendingRequests(); -> used by DMCore.Execute
        bool HasPendingTurnGroundingRequest(); -> used by HasPendingRequests
        bool HasPendingConceptGroundingRequests(); -> used by HasPendingRequests
        bool HasUnprocessedConceptGroundingRequests(); -> used by DMCore.Execute
        bool HasScheduledConceptGroundingRequests(); -> used by DMCore's Execute and assembleFocusClaims
        bool HasExecutingConceptGroundingRequests();
        bool GroundingInProgressOnConcept(CConcept* pConcept); -> used by Concept.IsUndergoingGrounding
        string GetScheduledGroundingActionOnConcept(CConcept* pConcept); -> used in performForcedConcepUpdates

        // Manage history
        void GAHAddHistoryItem(string sGroundingModelName, string sActionName, int iGroundingActionType); -> used in GroundingManagerAgent's Run
        void GAHSetBargeInFlag(int iTurnNum, bool bBargeInFlag);
        string GAHGetTurnGroundingAction(int iTurnNumber); -> used by GetGroundingFeature
        int GAHCountTakenInLastNTurns(bool bAlsoHeard, string sActionName, int iNumTurns);
        int GAHCountTakenByGroundingModelInLastNTurns(bool bAlsoHeard, string sActionName, string sGroundingModelName, int iNumTurns);

        void RegisterGroundingModelType(string sName, FCreateGroundingModel fctCreateGroundingModel);
        CGroundingModel* CreateGroundingModel(string sModelType, string sModelPolicy); -> CreateDialogTree diagram
        virtual void Run(); -> used by DMCore.Execute

        // Private API
        int getConceptGroundingRequestIndex(CConcept* pConcept); -> used throught the entire class
        string loadPolicy(string sFileName); -> used by LoadPoliciesFrom{String,File}

file: Grounding/GroundingModels/GroundingModel.h
    class CGroundingModel
    DATA:
        string sName; -> Core
        string sModelPolicy; -> used in subclasses
        TPolicy pPolicy; -> Used in ComputeActionValuesDistribution
        bool bExternalPolicy; -> Used in ComputeSuggestedActionIndex
        CExternalPolicyInterface* pepiExternalPolicy;
        string sExternalPolicyHost;
        vector<int> viActionMappings; -> used in subclasses and ComputeActionValuesDistribution
        string sExplorationMode; -> used in subclasses
        float fExplorationParameter; -> used in subclasses
        CState stFullState; -> used in subclasses
        CBeliefDistribution bdBeliefState; -> used in subclasses
        CBeliefDistribution bdActionValues; -> used in subclasses and ComputeActionValuesDistribution
        int iSuggestedActionIndex; -> result of the ComputeSuggestedActionIndex
    BEHAVIOUR:
        CGroundingModel(string sAModelPolicy, string sAModelName); -> Core
        CGroundingModel(CGroundingModel& rAGroundingModel); -> Core
        virtual ~CGroundingModel(); -> Core
        static CGroundingModel* GroundingModelFactory(string sModelPolicy); -> CreateDialogTree diagram
        virtual string GetType();
        virtual string GetModelPolicy();
        virtual string GetName();
        virtual void SetName(string sAName);
        virtual void Initialize(); -> CreateDialogTree diagram
        virtual CGroundingModel* Clone() = 0;
        virtual bool LoadPolicy(); -> used in Initialize
        virtual void ComputeState(); -> used in Run diagram by GroundingManagerAgent
        virtual void ComputeActionValuesDistribution(); -> can be defined in subclasses, used in ComputeSuggestedActionIndex
        virtual int ComputeSuggestedActionIndex(); -> can be defined in subclasses, used by the GroundingManagerAgent in Run
        virtual void Run();
        virtual void RunAction(int iActionIndex); -> Used by GroundingManagerAgent when the iActinIndex is computed and an action needs to be run
        virtual void LogStateAction(); -> used in GroundingManagerAgent, is subclassed
        virtual void computeFullState() = 0; -> defined in subclasses, used by ComputeState
        virtual void computeBeliefState() = 0; -> defined in subclasses, used by ComputeState
        string beliefStateToString(); -> used in LogStateAction
        string actionValuesToString(); -> used in LogStateAction and GMRequestAgent_LR

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
        virtual void RegisterDialogAgency() = 0; -> used by GroundingManager.UseGroundingAction

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
        vector<float> vfProbability; -> Core attribute
        vector<float> vfProbabilityLowBound; -> Core attribute
        vector<float> vfProbabilityHiBound; -> Core attribute
    BEHAVIOUR:
        CBeliefDistribution(int iNumEvents); -> Core
        CBeliefDistribution(CBeliefDistribution& rbdABeliefDistribution); -> Core
        ~CBeliefDistribution(); -> Core
        void Resize(int iNumEvents);
        float& operator[] (unsigned int iIndex);
        float& LowBound(unsigned int iIndex); -> Core
        float& HiBound(unsigned int iIndex); -> Core
        int GetValidEventsNumber(); -> Core
        void Normalize(); -> Core
        void Sharpen(float fPower); -> Core
        int GetModeEvent(); -> Core
        int GetMaxHiBoundEvent(); -> Core
        int GetEpsilonGreedyEvent(float fEpsilon); -> Core
        int GetSoftMaxEvent(float fTemperature); -> Core
        int GetRandomlyDrawnEvent(); -> Core

file: Agents/CoreAgents/StateManagerAgent.h
    class CStateManagerAgent : public CAgent
    DATA:
        STRING2STRING s2sDialogStateNames;
        vector<TDialogState, allocator<TDialogState>> vStateHistory; -> Core
        string sStateBroadcastAddress;
    BEHAVIOUR:
        CStateManagerAgent(string sAName, string sAConfiguration, string sAType); -> Core
        virtual ~CStateManagerAgent(); -> Core
        static CAgent* AgentFactory(string sAName, string sAConfiguration); -> Core
        virtual void Reset(); -> Core
        void LoadDialogStateNames(string sFileName); -> used by DMCore.InitializeDialogCore
        void SetStateBroadcastAddress(string sAStateBroadcastAddress);
        void BroadcastState(); -> used by AcquireNextEvent
        void UpdateState(); -> DMCore thread, AcquireNextEvent diagrams and StartOver
        string GetStateAsString(TDialogState dsState); -> used by GetStateAsString()
        string GetStateAsString(); -> used by BroadcastState and to log
        int GetStateHistoryLength(); -> used by the OutputManager
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
