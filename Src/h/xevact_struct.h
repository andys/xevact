                                     /*
                                   xevact
                       Associates actions with Events
                            Jon A. Maxwell (JAM)
                                     */



struct Action {
	Flag		flags;
	struct timeval	prev_time,delay;

	/* for function that tests for hit */
	int		tests;
	Condition	cond[MAX_TESTS];
	int		not[MAX_TESTS];
	int		(*match[MAX_TESTS])();

	/* for function that gets called for event-action */
	int	(*call)();
	char	*string;
	int	scalar;
	char	*call_ext_data;

	/* for ordering (NOT IMPLEMENTED!) */
	int	ref_count, priority;
	};


struct SetTable {
	int is_set;
	Mask mask;
	List tlist;
	struct Action default_action;
	};
struct StrToCallTable_Elem {
	char *name;
	int type;
	int (*call)();
	};
struct StrToConditionTable_Elem {
	char *name;
	Condition condition;
	};
	