
typedef enum _e_major_action
{
  MA_NOTHING,
  MA_PULSES_POS_NEG, // first positive, last negative
  MA_PULSES_NEG_POS, // first negative, last positive
} major_action_t;

typedef enum _e_minor_action
{
  MN_PULSE_PRE,
  MN_PULSE_POS,
  MN_PULSE_NEG,
  MN_PULSE_DONE
} minor_action_t;

struct pulse_cmd_t
{
  major_action_t major_action;
  timeval pre;
  timeval positive;
  timeval period;
  unsigned count;
};

struct pulse_params_t
{
  major_action_t major_action;
  minor_action_t minor_action;
  timeval start;
  timeval pre;
  timeval positive;
  timeval period;
  unsigned count;
};


void do_pulses(cond_list<pulse_cmd_t> *cmds)
{
  for (;;) {
    cmd->cond_wait();

    
  }
}
