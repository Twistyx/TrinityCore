//Credit: Immvp

#include "ScriptPCH.h"

enum Spells
{
    SPELL_1 = 51490,
    SPELL_2 = 0, //Replace "0" with ID of spell
    SPELL_3 = 0,
    SPELL_ENRAGE = 51513, //Enragespell
};

enum Events
{
    EVENT_1 = 1,
    EVENT_2 = 2,
    EVENT_3 = 3,
    EVENT_ENRAGE = 4,
    EVENT_SPAWNCREATURE = 5,
};

enum Phases
{
    PHASE_ONE = 1,
    PHASE_TWO = 2,
};

#define BLUE   "|cff96BBFF"
#define GREEN  "|cffABD473"
#define YELLOW "|cfffaeb00"
#define RED    "|cffC41F3B"


enum Yells
{
    YELL_AGGRO                      = 0,
    YELL_KILL                       = 1,
    YELL_DEATH                      = 2
};

class npc_immvp_boss : public CreatureScript
{
    public:
        npc_immvp_boss() : CreatureScript("npc_immvp_boss") { }

        struct npc_immvp_bossAI : public BossAI
        {
            npc_immvp_bossAI(Creature* creature) : BossAI(creature, 0)
            {
            }

            void Reset() OVERRIDE
            {
                _Reset();
            }
            
            void EnterCombat(Unit* /*who*/) OVERRIDE
            {
                Talk(YELL_AGGRO);
                // Time is in milliseconds. 1000 milliseconds = 1 second
                events.ScheduleEvent(EVENT_1, 15000, 0, PHASE_ONE);
                events.ScheduleEvent(EVENT_2, 12000);
                events.ScheduleEvent(EVENT_3, 10000);
                events.ScheduleEvent(EVENT_SPAWNCREATURE, 20000);
                events.SetPhase(PHASE_ONE);     
                events.ScheduleEvent(EVENT_ENRAGE, 14500, 0, PHASE_TWO);
            }
            
            void KilledUnit(Unit* /*victim*/) OVERRIDE
            {
                Talk(YELL_KILL);
            }
            
            void JustDied(Unit* /*victim*/) OVERRIDE
            {
                Talk(YELL_DEATH);
            }

            void JustRespawned() OVERRIDE
            {
                std::stringstream msg;
                msg << GREEN << "[World Boss]" << RED << " [The Awakened Chaos]" << YELLOW <<"I am back.";

                sWorld->SendServerMessage(SERVER_MSG_STRING, msg.str().c_str());
                Reset();
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;
              
                if (events.IsInPhase(PHASE_ONE) && HealthBelowPct(50))
                    events.SetPhase(PHASE_TWO);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_1:
                        {
                            // Will cast the spell on the victim (main tank.)
                            DoCastVictim(SPELL_1);

                            // Will cast the spell on the victim (main tank.)
                            events.ScheduleEvent(EVENT_1, 15000);
                            break;
                        }
                        case EVENT_2:
                        {
                            //Will cast the spell on itself.
                            DoCast(me, SPELL_2);

                            //Will execute the event again after 12000 milliseconds (12 seconds.)
                            events.ScheduleEvent(EVENT_2, 12000);
                            break;
                        }
                        case EVENT_3:
                        {

                            Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0);

                            if (!target)
                                break;

                            DoCast(target, SPELL_3);
                            events.ScheduleEvent(EVENT_3, 10000);
                            
                            break;
                        }
                        case EVENT_ENRAGE:
                        {
                            // Will cast the spell on the victim (main tank.)
                            DoCast(me, SPELL_ENRAGE);

                            // Will cast the spell on the victim (main tank.)
                            events.ScheduleEvent(EVENT_ENRAGE, 15000);
                            break;
                        }
                        default:
                            break;
                    }
                }
                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_immvp_bossAI(creature);
        }
};

void AddSC_custom_immvp_scripts()
{
    new npc_immvp_boss();
}
