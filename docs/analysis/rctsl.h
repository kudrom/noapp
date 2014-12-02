file: DMCore/Grounding/GroundingActions/AllGroundingActions.h
    #define ALL_GROUNDING_ACTIONS(Configuration)\
        GROUNDING_ACTION(NO_ACTION, NO_ACTION, Configuration)\
        GROUNDING_ACTION(ACCEPT, ACCEPT, Configuration)\
        GROUNDING_ACTION(EXPL_CONF, EXPLICIT_CONFIRM, Configuration)\
        GROUNDING_ACTION(IMPL_CONF, IMPLICIT_CONFIRM, Configuration)\
        GROUNDING_ACTION(ASK_REPEAT, ASK_REPEAT, Configuration)\
        GROUNDING_ACTION(NOTIFY_AND_ASK_REPEAT, ASK_REPEAT, Configuration + ", notify=true")\
        GROUNDING_ACTION(ASK_REPHRASE, ASK_REPHRASE, Configuration)\
        GROUNDING_ACTION(NOTIFY_AND_ASK_REPHRASE, ASK_REPHRASE, Configuration + ", notify=true")\
        GROUNDING_ACTION(REPEAT_PROMPT, REPEAT_PROMPT, Configuration)\
        GROUNDING_ACTION(NOTIFY_AND_REPEAT_PROMPT, REPEAT_PROMPT, Configuration + ", notify=true")\
        GROUNDING_ACTION(EXPLAIN_MORE, EXPLAIN_MORE, Configuration)\
        GROUNDING_ACTION(NOTIFY_AND_EXPLAIN_MORE, EXPLAIN_MORE, Configuration + ", notify=true")\
        GROUNDING_ACTION(FULL_HELP, FULL_HELP, Configuration)\
        GROUNDING_ACTION(NOTIFY_AND_FULL_HELP, FULL_HELP, Configuration + ", notify=true")\
        GROUNDING_ACTION(WHAT_CAN_I_SAY, WHAT_CAN_I_SAY, Configuration)\
        GROUNDING_ACTION(TERSE_WHAT_CAN_I_SAY, WHAT_CAN_I_SAY, Configuration + ", terse=true")\
        GROUNDING_ACTION(NOTIFY_AND_WHAT_CAN_I_SAY, WHAT_CAN_I_SAY, Configuration + ", notify=true")\
        GROUNDING_ACTION(NOTIFY_AND_TERSE_WHAT_CAN_I_SAY, WHAT_CAN_I_SAY, Configuration + ", terse=true, notify=true")\
        GROUNDING_ACTION(TIPS_AND_WHAT_CAN_I_SAY, WHAT_CAN_I_SAY, Configuration + ", tips=true")\
        GROUNDING_ACTION(NOTIFY_NONUNDERSTANDING, NOTIFY_NONUNDERSTANDING, Configuration)\
        GROUNDING_ACTION(YIELD_TURN, YIELD_TURN, Configuration)\
        GROUNDING_ACTION(FAIL_REQUEST, FAIL_REQUEST, Configuration)\
        GROUNDING_ACTION(ASK_STARTOVER, ASK_STARTOVER, Configuration)\
        GROUNDING_ACTION(SPEAK_LESS_LOUD_AND_REPROMPT, SPEAK_LESS_LOUD_AND_REPROMPT, Configuration)\
        GROUNDING_ACTION(ASK_SHORT_ANSWER_AND_REPROMPT, ASK_SHORT_ANSWER_AND_REPROMPT, Configuration)\
        GROUNDING_ACTION(ASK_SHORT_ANSWER_AND_WHAT_CAN_I_SAY, ASK_SHORT_ANSWER_AND_WHAT_CAN_I_SAY, Configuration)\
        GROUNDING_ACTION(INTERACTION_TIPS, INTERACTION_TIPS, Configuration)\
        GROUNDING_ACTION(GIVE_UP, GIVE_UP, Configuration)\
        GROUNDING_ACTION(MOVE_ON, MOVE_ON, Configuration)

file: DMCore/Concepts/ArrayConcept.h
    #define DEFINE_ARRAY_CONCEPT_TYPE(ElementaryConceptType, ArrayConceptTypeName) \
        class ArrayConceptTypeName: public CArrayConcept {\
        public:\
            ArrayConceptTypeName(string aName = "NONAME",\
                                 TConceptSource csAConceptSource = csUser): \
                CArrayConcept(aName, csAConceptSource) {\
            }\
            virtual CConcept* CreateElement() {\
                CConcept* pConcept;\
                pConcept = new ElementaryConceptType("NONAME", csConceptSource);\
                pConcept->CreateGroundingModel(sElementGroundingModelSpec);\
                return pConcept;\
            }\
            virtual CConcept* EmptyClone() {\
                return new ArrayConceptTypeName;\
            }\
        };\

file: DMCore/Concepts/Concept.h
    // D: Macro for defining a derived concept type 
    #define DEFINE_CONCEPT_TYPE(NewConceptTypeName, BaseConceptTypeName, OTHER_CONTENTS)\
        class NewConceptTypeName: public BaseConceptTypeName {\
        public:\
            NewConceptTypeName(string sAName = "NONAME",\
                               TConceptSource csAConceptSource = csUser) :\
                BaseConceptTypeName(sAName, csAConceptSource) {};\
            virtual CConcept* EmptyClone() {\
                return new NewConceptTypeName;\
            };\
            OTHER_CONTENTS;\
        };\

    // D: macro for defining the prior for a hypothesis
    #define DEFINE_PRIOR(CODE)\
        public:\
        virtual float GetPriorForHyp(CHyp* pHyp) {\
            CODE;\
        }\

    // D: macro for defining the confusability for a hypothesis
    #define DEFINE_CONFUSABILITY(CODE)\
        public:\
        virtual float GetConfusabilityForHyp(CHyp* pHyp) {\
            CODE;\
        }\

file: DMCore/Concepts/FrameConcept.h
    // D: Macro for defining a derived structure concept class
    #define DEFINE_FRAME_CONCEPT_TYPE(FrameConceptTypeName, OTHER_CONTENTS) \
        class FrameConceptTypeName: public CFrameConcept {\
        public:\
            FrameConceptTypeName(string aName = "NONAME",\
                                 TConceptSource csAConceptSource = csUser):\
                CFrameConcept(aName, csAConceptSource) {\
                CreateStructure();\
            };\
            virtual CConcept* EmptyClone() {\
                return new FrameConceptTypeName;\
            };\
            OTHER_CONTENTS\
        };\

file: DMCore/Agents/DialogAgents/DialogAgent.h
    // D: macro for defining preconditions for agents
    #define PRECONDITION(Condition)\
        public:\
        virtual bool PreconditionsSatisfied() {\
            return (Condition);\
        }\
    
    // D: macro for defining preconditions that are the same as the triggers
    #define SAME_AS_TRIGGER \
        ClaimsFocus()\
        
    // D: macro which indicates that the agent can claim the focus during grounding
    #define CAN_TRIGGER_DURING_GROUNDING \
        public:\
        virtual bool ClaimsFocusDuringGrounding() {\
            return true;\
        }\
    
    // D: macro for defining success criteria (when an agency completes successfully)
    #define SUCCEEDS_WHEN(Condition)\
        public:\
        virtual bool SuccessCriteriaSatisfied() {\
            return (Condition);\
        }\
    
    // D: macro for defining failure criteria (when an agency completes with failure)
    #define FAILS_WHEN(Condition)\
        public:\
        virtual bool FailureCriteriaSatisfied() {\
            return (Condition);\
        }\
    
    // D: macro for defining the maximum number of attempts for an agent
    #define MAX_ATTEMPTS(Count)\
        public:\
        virtual int GetMaxExecuteCounter() {\
            return (Count);\
        }\
    
    // D: macro for defining the concept binding policy
    #define CONCEPT_BINDING_POLICY(Policy)\
        public:\
        virtual string DeclareBindingPolicy() {\
            return (Policy);\
        }\
    
    // D: defining the expect condition
    #define EXPECT_WHEN(Condition)\
        public:\
        virtual bool ExpectCondition() {\
            return(Condition);\
        };\
    
    // D: macro for specifying code on the creation of each agent
    #define ON_CREATION(DO_STUFF)\
        public:\
        virtual void OnCreation() {\
            DO_STUFF;\
        }\
    
    // D: macro for specifying code on the destruction of each agent
    #define ON_DESTRUCTION(DO_STUFF)\
        public:\
        virtual void OnDestruction() {\
            DO_STUFF;\
        }\
    
    // D: macro for specifying initialization code for an agent
    #define ON_INITIALIZATION(DO_STUFF)\
        public:\
        virtual void OnInitialization() {\
            DO_STUFF;\
        }\
    
    // D: macro for defining an "effect" for the agent, through the OnCompletion
    //    virtual function
    #define ON_COMPLETION(DO_STUFF)\
        public:\
        virtual void OnCompletion() {\
            DO_STUFF;\
        }\
    
    // D: macro for defining reopening behavior for a dialog agent
    #define ON_REOPEN(DO_STUFF)\
        public:\
        virtual void ReOpen() {\
            DO_STUFF;\
        }\
    
    
    // D: macro to be used to reset an agent on completion
    #define RESET {Reset();}
    #define BLOCK {Block();}
    #define REOPEN {ReOpen();}
    
    // D: macro for defining the conditions under which the agent claims focus
    #define TRIGGERED_BY(Condition)\
        public:\
        virtual bool ClaimsFocus() {\
            return (Condition);\
        }\
    
    // D: macro for definiting the user commands which trigger this agent
    #define TRIGGERED_BY_COMMANDS(Commands, GroundingModelSpec)\
        public:\
        virtual string TriggeredByCommands() {\
            sTriggeredByCommands = Commands;\
            sTriggerCommandsGroundingModelSpec = GroundingModelSpec;\
            return (Commands);\
        }\
    
    // A: macro for defining agents which require the floor to be executed
    #define REQUIRES_FLOOR()\
        public:\
        virtual bool RequiresFloor() {\
            return true;\
        }\
        
    // A: macro for defining agents which do not require the floor in order to be executed
    #define DOES_NOT_REQUIRE_FLOOR()\
        public:\
        virtual bool RequiresFloor() {\
            return false;\
        }\
        
    // D: macro for defining agents which are main topics
    #define IS_MAIN_TOPIC()\
        public:\
        virtual bool IsAMainTopic() {\
            return true;\
        }\
        
    // D: macro for defining agents which are main topics
    #define IS_NOT_DTS_AGENT()\
        public:\
        virtual bool IsDTSAgent() {\
            return false;\
        }\
    
    // D: macro for defining the prompt
    #define PROMPT(PROMPT)\
        public:\
        virtual string Prompt() {\
            return (PROMPT);\
        }\
    
    // D: macro for defining the timeout prompt
    #define PROMPT_TIMEOUT(PROMPT)\
        public:\
        virtual string TimeoutPrompt() {\
            return (PROMPT);\
        }\
    
    // D: macro for defining the explain more prompt
    #define PROMPT_EXPLAINMORE(PROMPT)\
        public:\
        virtual string ExplainMorePrompt() {\
            return (PROMPT);\
        }\
    
    // D: macro for defining the establish context prompt
    #define PROMPT_ESTABLISHCONTEXT(PROMPT)\
        public:\
        virtual string EstablishContextPrompt() {\
            return (PROMPT);\
        }\
    
    // D: macro for defining the what can i say prompt
    #define PROMPT_WHATCANISAY(PROMPT)\
        public:\
        virtual string WhatCanISayPrompt() {\
            return (PROMPT);\
        }\
    
    // AR: macro for defining the language model required by the agent
    #define INPUT_LINE_CONFIGURATION(CONFIG_LINE)\
        public:\
        virtual string InputLineConfigurationInitString() {\
            return (CONFIG_LINE);\
        }\
    
    // TK: constant intended as a particular INPUT_LINE_CONFIGURATION CONFIG_LINE
    // Causes the next input to be a raw audio 
    #define RAW_AUDIO_CONFIG "raw_audio=1"
    
    // A: macro for defining agents which cannot be executed by anticipation
    #define IS_CONVERSATION_SYNCHRONOUS()\
        public:\
        virtual bool IsConversationSynchronous() {\
            return true;\
        }\

file: DMCore/Agents/DialogAgents/BasicAgents/MAExecute.h
    // D: defining an execute agent
    #define DEFINE_EXECUTE_AGENT(ExecuteAgentClass, OTHER_CONTENTS)\
        class ExecuteAgentClass: public CMAExecute {\
        public:\
            ExecuteAgentClass(string sAName,\
                              string sAType = "CAgent:CDialogAgent:CMAExecute:"#ExecuteAgentClass):\
                CMAExecute(sAName, sAType) {;}\
            ~ExecuteAgentClass() {;}\
            static CAgent* AgentFactory(string sAName, string sAConfiguration) {\
                return new ExecuteAgentClass(sAName, sAConfiguration);\
            }\
            OTHER_CONTENTS\
        };\
    
    // D: defining the execute routine of an execute agent
    #define EXECUTE(CONTENTS)\
        public:\
        virtual void ExecuteRoutine() {\
            CONTENTS;\
        }\
    
    // D: defining the call of an execute agent (within an execute agent definition)
    #define CALL(String)\
        public:\
        virtual string GetExecuteCall() {\
            return String;\
        };\

file: DMCore/Agents/DialogAgents/BasicAgents/MAExpect.h
    // D: defining an expect agent
    #define DEFINE_EXPECT_AGENT(ExpectAgentClass, OTHER_CONTENTS)\
        class ExpectAgentClass: public CMAExpect {\
        public:\
            ExpectAgentClass(string sAName,\
                             string sAType = "CAgent:CDialogAgent:CMAExpect:"#ExpectAgentClass):\
                CMAExpect(sAName, sAType) {;}\
            ~ExpectAgentClass() {;}\
            static CAgent* AgentFactory(string sAName, string sAConfiguration) {\
                return new ExpectAgentClass(sAName, sAConfiguration);\
            }\
            OTHER_CONTENTS\
        };\
    
    // D: defining the concept of an expect agent
    #define EXPECT_CONCEPT(ConceptName)\
        public:\
        virtual string ExpectedConceptName() {\
        return(#ConceptName);\
        };\
    
    // D: defining the concept mapping used by the expectation agent
    #define GRAMMAR_MAPPING(GrammarMappingAsString)\
        public:\
        virtual string GrammarMapping() {\
            return (string)(GrammarMappingAsString);\
        };\

file: DMCore/Agents/DialogAgents/BasicAgents/MAInform.h
    // D: defining an inform agent
    #define DEFINE_INFORM_AGENT(InformAgentClass, OTHER_CONTENTS)\
        class InformAgentClass: public CMAInform {\
        public:\
            InformAgentClass(string sAName,\
                             string sAType = "CAgent:CDialogAgent:CMAInform:"#InformAgentClass):\
                CMAInform(sAName, sAType) {;}\
            ~InformAgentClass() {;}\
            static CAgent* AgentFactory(string sAName, string sAConfiguration) {\
                return new InformAgentClass(sAName, sAConfiguration);\
            }\
            OTHER_CONTENTS\
        };\

file: DMCore/Agents/DialogAgents/BasicAgents/MARequest.h
    // D: defining a request agent
    #define DEFINE_REQUEST_AGENT(RequestAgentClass, OTHER_CONTENTS)\
        class RequestAgentClass: public CMARequest {\
        public:\
            RequestAgentClass(string sAName,\
                              string sAType = "CAgent:CDialogAgent:CMARequest:"#RequestAgentClass): \
                 CMARequest(sAName, sAType) {;}\
            ~RequestAgentClass() {;}\
            static CAgent* AgentFactory(string sAName, string sAConfiguration) {\
                return new RequestAgentClass(sAName, sAConfiguration);\
            }\
            OTHER_CONTENTS\
        };\
    
    // D: defining the concept of a request agent
    #define REQUEST_CONCEPT(ConceptName)\
        public:\
        virtual string RequestedConceptName() {\
            return(ReplaceSubString(#ConceptName, "#", GetDynamicAgentID()));\
        };\
        
    // D: defining the concept mapping used by the request agent
    #define GRAMMAR_MAPPING(GrammarMappingAsString)\
        public:\
        virtual string GrammarMapping() {\
            return (string)(GrammarMappingAsString);\
        };\
    
    // D: defining the timeout duration for the request agent in seconds
    #define TIMEOUT_PERIOD(Period)\
        public:\
        virtual int GetTimeoutPeriod() {\
            return(Period);\
        };\
    
    // D: defining the rejection/nonundersatnding threhold
    #define NONUNDERSTANDING_THRESHOLD(Threshold)\
        public:\
        virtual float GetNonunderstandingThreshold() {\
            return (Threshold);\
        };\

file: DMCore/Agents/DialogAgents/BasicAgents/DialogAgency.h
    // D: macro for agency definition
    #define DEFINE_AGENCY(AgencyClass, OTHER_CONTENTS)\
        class AgencyClass: public CDialogAgency {\
        public:\
        AgencyClass(string sAName, \
            string sAType = "CAgent:CDialogAgent:CDialogAgency:"#AgencyClass): \
            CDialogAgency(sAName, sAType) {\
            }\
            virtual ~AgencyClass() {;}\
            static CAgent* AgentFactory(string sAName, string sAConfiguration) {\
                return new AgencyClass(sAName, sAConfiguration);\
            }\
            OTHER_CONTENTS\
        };\
        
    
    // D: macro for defining an execution policy
    #define EXECUTION_POLICY(CONTENTS) \
        public:\
            virtual CDialogAgent* NextAgentToExecute() {\
                CONTENTS;\
            }\
    
    // D: macro for subagents definition section within an agency
    #define DEFINE_SUBAGENTS(SUBAGENTS)\
        public:\
            virtual void CreateSubAgents() {\
                CDialogAgent* pNewAgent;\
                SUBAGENTS\
            }\
    
    // D: macros for defining a subagent within the subagent definition section
    #define SUBAGENT(SubAgentName, SubAgentType, GroundingModelSpec)\
                pNewAgent = (CDialogAgent *)\
                            AgentsRegistry.CreateAgent(#SubAgentType, \
                                                       #SubAgentName);\
                pNewAgent->SetParent(this);\
                pNewAgent->CreateGroundingModel(GroundingModelSpec);\
                SubAgents.push_back(pNewAgent);\
                pNewAgent->Initialize();\
    
    // D: macro for concepts definition section within an agency
    #define DEFINE_CONCEPTS(CONCEPTS)\
        public:\
            virtual void CreateConcepts() {\
                CONCEPTS\
            }\
    
    // D: macro for defining a concept within the concept definition section
    #define USER_CONCEPT(ConceptName, ConceptType, GroundingModelSpec)\
        Concepts.push_back(new ConceptType(#ConceptName, csUser));\
        Concepts.back()->CreateGroundingModel(GroundingModelSpec);\
        Concepts.back()->SetOwnerDialogAgent(this);\
    
    #define SYSTEM_CONCEPT(ConceptName, ConceptType)\
        Concepts.push_back(new ConceptType(#ConceptName, csSystem));\
        Concepts.back()->CreateGroundingModel("none");\
        Concepts.back()->SetOwnerDialogAgent(this);\

file: DMCore/Agents/CoreAgents/DTTManagerAgent.h
    // D: macro for defining the dialog task root
    #define DECLARE_DIALOG_TASK_ROOT(RootAgentName, RootAgentType, \
        GroundingModelSpec)\
        void CDTTManagerAgent::CreateDialogTaskTree() {\
            Log(DTTMANAGER_STREAM, "Creating Dialog Task Tree ...");\
            // TODO: This is using a C type casting \
            pdaDialogTaskRoot = (CDialogAgent *)\
                                AgentsRegistry.CreateAgent(#RootAgentType,\
                                                           #RootAgentName);\
            pdaDialogTaskRoot->SetParent(NULL);\
            // TODO: Why here? \
            pdaDialogTaskRoot->CreateGroundingModel(GroundingModelSpec);\
            pdaDialogTaskRoot->Initialize();\
            pdaDialogTaskRoot->Register();\
        }\
    
    // D: macro for declaring the agents (defining and registering all the 
    //    agent types in a task
    #define DECLARE_AGENTS(DECLARATIONS)\
        void CDTTManagerAgent::CreateDialogTaskAgentome() {\
            Log(DTTMANAGER_STREAM, "Registering  dialog task agent types ...");\
            DECLARATIONS\
        }\
    
    // D: macro for registering a particular agent type - to be used within a 
    //    DECLARE_AGENTS declaration
    #define DECLARE_AGENT(AgentClass)\
        AgentsRegistry.RegisterAgentType(#AgentClass, \
                                         AgentClass::AgentFactory);\

file: DialogTask/DialogTask.h
    // D: the define for the galaxy server configuration
    #define DMSERVER_CONFIGURATION(Name, Port)\
        extern "C" {\
            char* lpszDMServerName = Name;\
            int iDMServerPort = Port;\
        }\
    
    
    // D: the define for the DialogTaskOnBeginSession function
    #define CORE_CONFIGURATION(Configuration)\
    	void DialogTaskOnBeginSession() {\
            Configuration\
    	}\
    
    
    // D: define the name of the slot containing the confidence information from 
    // helios
    // TK: DEPRECATED
    #define DEFINE_CONFIDENCE_SLOTNAME(Name)\
        ;\
    
    // D: define for declaring output devices
    #define USE_OUTPUT_DEVICES(Devices)\
    	{\
    		Devices\
    	}\
    
    // D: define for declaring a certain output device
    #define OUTPUT_DEVICE(Name, ServerCall, Params)\
    	pOutputManager->RegisterOutputDevice(Name, ServerCall, Params);\
    
    // D: define for declaring the default output device
    #define DEFAULT_OUTPUT_DEVICE(Name, ServerCall, Params)\
    	pOutputManager->RegisterOutputDevice(Name, ServerCall, Params);\
    	pOutputManager->SetDefaultOutputDevice(Name);\
    
    // D: define for the output class
    #define USE_OUTPUT_CLASS(OutputClass)\
        pOutputManager->SetOutputClass(#OutputClass);\
    
    // D: define the default timeout period
    #define DEFAULT_TIMEOUT_PERIOD(Period)\
    	pDMCore->SetDefaultTimeoutPeriod(Period);\
    
    // D: define the library agents to be used
    #define USE_LIBRARY_AGENTS(Agents)\
        {\
            Agents\
        }\
    
    // D: declare which library agents to be used
    #define LIBRARY_AGENT(AgentType, AgentName, RegistrationFunction, ConfigurationString)\
    	pDTTManager->Use(#AgentType, #AgentName, &RegistrationFunction, ConfigurationString);
    
    // D: define the grounding model types to use
    #define USE_GROUNDING_MODEL_TYPES(ModelTypes)\
        {\
            ModelTypes\
        }\
    
    // D: define the grounding actions to be used
    #define USE_GROUNDING_ACTIONS(Actions)\
        {\
            Actions\
        }\
    
    // D: define for using all the grounding model types
    #define USE_ALL_GROUNDING_MODEL_TYPES\
        USE_GROUNDING_MODEL_TYPES(ALL_GROUNDING_MODEL_TYPES)
    
    // D: declare which grounding models are used
    #define GROUNDING_MODEL_TYPE(Name, FactoryMethod)\
        pGroundingManager->RegisterGroundingModelType(#Name, FactoryMethod);
    
    // D: define for using all the grounding actions
    #define USE_ALL_GROUNDING_ACTIONS(Configuration)\
        USE_GROUNDING_ACTIONS(ALL_GROUNDING_ACTIONS(Configuration))
    
    // D: declare which grounding actions are to be used
    #define GROUNDING_ACTION(Name, GAType, Configuration)\
        pGroundingManager->UseGroundingAction(#Name, new GAType((string)Configuration));
    
    // D: declare the binding filters to be used
    #define USE_BINDING_FILTERS(BindingFilters)\
        {\
            BindingFilters\
        }\
    
    // D: declare customized binding filters to be user
    #define BINDING_FILTER(Name, Filter)\
        pDMCore->RegisterBindingFilter(Name, Filter);\
        
    // D: declare a customized start over routine
    #define CUSTOM_START_OVER(Funct)\
        pDMCore->RegisterCustomStartOver(Funct);\
    
    // D: macro for logging from the dialog task log
    #define LOG(X)\
        Log(DIALOGTASK_STREAM, X);\
    
    //-----------------------------------------------------------------------------
    // 
    // Other macros to increase readability of dialog task specification
    //
    //-----------------------------------------------------------------------------
    
    // D: shortcuts for definiting items in structures
    #define INT_ITEM(Name) \
        ITEM(Name, CIntConcept)
    #define BOOL_ITEM(Name) \
        ITEM(Name, CBoolConcept)
    #define STRING_ITEM(Name) \
        ITEM(Name, CStringConcept)
    #define FLOAT_ITEM(Name) \
        ITEM(Name, CFloatConcept)
    #define CUSTOM_ITEM(Name, Type) \
        ITEM(Name, Type)
    
    // D: shortcuts for defining concepts
    #define INT_SYSTEM_CONCEPT(Name) SYSTEM_CONCEPT(Name, CIntConcept)
    #define BOOL_SYSTEM_CONCEPT(Name) SYSTEM_CONCEPT(Name, CBoolConcept)
    #define STRING_SYSTEM_CONCEPT(Name) SYSTEM_CONCEPT(Name, CStringConcept)
    #define FLOAT_SYSTEM_CONCEPT(Name) SYSTEM_CONCEPT(Name, CFloatConcept)
    #define CUSTOM_SYSTEM_CONCEPT(Name, Type) SYSTEM_CONCEPT(Name, Type)
    
    // D: shortcuts for various agent definitions
    #define RESET_ON_COMPLETION ON_COMPLETION(RESET)
    #define REOPEN_ON_COMPLETION ON_COMPLETION(REOPEN)
    
    #define INT_USER_CONCEPT(Name, GroundingModelSpec) \
        USER_CONCEPT(Name, CIntConcept, GroundingModelSpec)
    #define BOOL_USER_CONCEPT(Name, GroundingModelSpec) \
        USER_CONCEPT(Name, CBoolConcept, GroundingModelSpec)
    #define STRING_USER_CONCEPT(Name, GroundingModelSpec) \
        USER_CONCEPT(Name, CStringConcept, GroundingModelSpec)
    #define FLOAT_USER_CONCEPT(Name, GroundingModelSpec) \
        USER_CONCEPT(Name, CFloatConcept, GroundingModelSpec)
    #define CUSTOM_USER_CONCEPT(Name, Type, GroundingModelSpec) \
        USER_CONCEPT(Name, Type, GroundingModelSpec)
    
    // D: shortcuts for various agent check functions
    #define COMPLETED(Agent) (A(#Agent).HasCompleted())
    #define FAILED(Agent) (A(#Agent).HasFailed())
    #define SUCCEEDED(Agent) (A(#Agent).HasSucceeded())
    #define IS_ACTIVE_TOPIC(Agent) (pDMCore->AgentIsActive(&(A(#Agent))))
    #define IS_FOCUSED(Agent) (pDMCore->IsFocused(&(A(#Agent))))
    
    // D: shortcuts for commands on agents
    #define SET_COMPLETED(Agent) {A(#Agent).SetCompleted();}
    #define FINISH(Agent) {A(#Agent).SetCompleted(); \
        pDMCore->PopAgentFromExecutionStack(&A(#Agent));}
    #define RESET_AGENT(Agent) {A(#Agent).Reset();}
    #define REOPEN_AGENT(Agent) {A(#Agent).ReOpen();}
    #define REOPEN_TOPIC(Agent) {A(#Agent).ReOpenTopic();}
    
    // D: shortcuts for various concept check functions
    #define AVAILABLE(Concept) (C(#Concept).IsAvailableAndGrounded())
    #define UNAVAILABLE(Concept) (!(C(#Concept).IsAvailableAndGrounded()))
    #define UPDATED(Concept) (C(#Concept).IsUpdatedAndGrounded())
    #define NOT_UPDATED(Concept) (!(C(#Concept).IsUpdatedAndGrounded()))
    #define INVALIDATED(Concept) (C(#Concept).IsInvalidated())
    #define IS_TRUE(Concept) (AVAILABLE(Concept) && (bool)C(#Concept))
    #define IS_FALSE(Concept) (AVAILABLE(Concept) && !(bool)C(#Concept))
    #define HAS_HISTORY(Concept) (C(#Concept).GetHistorySize() > 0)
    #define TURNS_SINCE_LAST_UPDATED(Concept) (C(#Concept).GetTurnsSinceLastUpdated())
    #define UPDATED_IN_LAST_TURN(Concept) ((C(#Concept).GetTurnsSinceLastUpdated())==0)
    #define UPDATED_IN_PENULTIMATE_TURN(Concept) ((C(#Concept).GetTurnsSinceLastUpdated())==1)
    
    // D: shortcuts for methods on concepts
    #define REOPEN_CONCEPT(Concept) {C(#Concept).ReOpen();}
    #define RESTORE_CONCEPT(Concept) {C(#Concept).Restore();}
    #define CLEAR(Concept) {C(#Concept).Clear();}
    #define SIZE(Concept) (C(#Concept).GetSize())
    
    #endif // __DIALOGTASK_H__
