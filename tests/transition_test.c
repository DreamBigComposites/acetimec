#include <acunit.h>
#include <acetimec.h>

ACU_TEST(test_atc_transition_compare_to_match_fuzzy)
{
  const AtcMatchingEra match = {
    {2000, 1, 1, 0, kAtcSuffixW} /* start_dt */,
    {2001, 1, 1, 0, kAtcSuffixW} /* until_dt */,
    NULL /*era*/,
    NULL /*prev_match*/,
    0 /*last_offset_seconds*/,
    0 /*last_delta_seconds*/
  };

  AtcTransition transition = {
    &match /*match*/,
    NULL /*rule*/,
    {1999, 11, 1, 0, kAtcSuffixW} /*transition_time*/,
    {{0, 0, 0, 0, 0}} /*start_dt*/,
    {{0, 0, 0, 0, 0}} /*until_dt*/,
    0 /*start_epoch_seconds*/,
    0 /*offset_seconds*/,
    0 /*delta_seconds*/,
    {0} /*abbrev*/,
    NULL /*letter*/,
    {0} /*match_status*/
  };
  uint8_t status = atc_transition_compare_to_match_fuzzy(&transition, &match);
  ACU_ASSERT(status == kAtcComparePrior);

  transition = (AtcTransition) {
    &match /*match*/,
    NULL /*rule*/,
    {1999, 12, 1, 0, kAtcSuffixW} /*transition_time*/,
    {{0, 0, 0, 0, 0}} /*start_dt*/,
    {{0, 0, 0, 0, 0}} /*until_dt*/,
    0 /*start_epoch_seconds*/,
    0 /*offset_seconds*/,
    0 /*delta_seconds*/,
    {0} /*abbrev*/,
    NULL /*letter*/,
    {0} /*match_status*/
  };
  status = atc_transition_compare_to_match_fuzzy(&transition, &match);
  ACU_ASSERT(status == kAtcCompareWithinMatch);

  transition = (AtcTransition) {
    &match /*match*/,
    NULL /*rule*/,
    {2000, 1, 1, 0, kAtcSuffixW} /*transition_time*/,
    {{0, 0, 0, 0, 0}} /*start_dt*/,
    {{0, 0, 0, 0, 0}} /*until_dt*/,
    0 /*start_epoch_seconds*/,
    0 /*offset_seconds*/,
    0 /*delta_seconds*/,
    {0} /*abbrev*/,
    NULL /*letter*/,
    {0} /*match_status*/
  };
  status = atc_transition_compare_to_match_fuzzy(&transition, &match);
  ACU_ASSERT(status == kAtcCompareWithinMatch);

  transition = (AtcTransition) {
    &match /*match*/,
    NULL /*rule*/,
    {2001, 1, 1, 0, kAtcSuffixW} /*transition_time*/,
    {{0, 0, 0, 0, 0}} /*start_dt*/,
    {{0, 0, 0, 0, 0}} /*until_dt*/,
    0 /*start_epoch_seconds*/,
    0 /*offset_seconds*/,
    0 /*delta_seconds*/,
    {0} /*abbrev*/,
    NULL /*letter*/,
    {0} /*match_status*/
  };
  status = atc_transition_compare_to_match_fuzzy(&transition, &match);
  ACU_ASSERT(status == kAtcCompareWithinMatch);

  transition = (AtcTransition) {
    &match /*match*/,
    NULL /*rule*/,
    {2001, 3, 1, 0, kAtcSuffixW} /*transition_time*/,
    {{0, 0, 0, 0, 0}} /*start_dt*/,
    {{0, 0, 0, 0, 0}} /*until_dt*/,
    0 /*start_epoch_seconds*/,
    0 /*offset_seconds*/,
    0 /*delta_seconds*/,
    {0} /*abbrev*/,
    NULL /*letter*/,
    {0} /*match_status*/
  };
  status = atc_transition_compare_to_match_fuzzy(&transition, &match);
  ACU_ASSERT(status == kAtcCompareFarFuture);
}

ACU_TEST(test_atc_transition_compare_to_match)
{
  // UNTIL = 2002-01-02T03:00
  const AtcZoneEra ERA = {
      NULL /*zonePolicy*/,
      "" /*format*/,
      0 /*offsetCode*/,
      0 /*deltaCode*/,
      2 /*untilYearTiny*/,
      1 /*untilMonth*/,
      2 /*untilDay*/,
      12 /*untilTimeCode*/,
      kAtcSuffixW
  };

  // MatchingEra=[2000-01-01, 2001-01-01)
  const AtcMatchingEra match = {
    {2000, 1, 1, 0, kAtcSuffixW} /*startDateTime*/,
    {2001, 1, 1, 0, kAtcSuffixW} /*untilDateTime*/,
    &ERA /*era*/,
    NULL /*prevMatch*/,
    0 /*lastOffsetMinutes*/,
    0 /*lastDeltaMinutes*/
  };

  // transitionTime = 1999-12-31
  AtcTransition transition0 = {
    &match /*match*/,
    NULL /*rule*/,
    {1999, 12, 31, 0, kAtcSuffixW} /*transitionTime*/,
    {{0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0}},
    0, 0, 0, {0}, NULL /*letter*/,
    {0}
  };

  // transitionTime = 2000-01-01
  AtcTransition transition1 = {
    &match /*match*/,
    NULL /*rule*/,
    {2000, 1, 1, 0, kAtcSuffixW} /*transitionTime*/,
    {{0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0}},
    0, 0, 0, {0}, NULL /*letter*/,
    {0}
  };

  // transitionTime = 2000-01-02
  AtcTransition transition2 = {
    &match /*match*/,
    NULL /*rule*/,
    {2000, 1, 2, 0, kAtcSuffixW} /*transitionTime*/,
    {{0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0}},
    0, 0, 0, {0}, NULL /*letter*/,
    {0}
  };

  // transitionTime = 2001-02-03
  AtcTransition transition3 = {
    &match /*match*/,
    NULL /*rule*/,
    {2001, 2, 3, 0, kAtcSuffixW} /*transitionTime*/,
    {{0, 0, 0, 0, 0}},
    {{0, 0, 0, 0, 0}},
    0, 0, 0, {0}, NULL /*letter*/,
    {0}
  };

  AtcTransition *transitions[] = {
    &transition0,
    &transition1,
    &transition2,
    &transition3,
  };

  // Populate the transitionTimeS and transitionTimeU fields.
  atc_transition_fix_times(&transitions[0], &transitions[4]);

  uint8_t status = atc_transition_compare_to_match(&transition0, &match);
  ACU_ASSERT(status == kAtcComparePrior);

  status = atc_transition_compare_to_match(&transition1, &match);
  ACU_ASSERT(status == kAtcCompareExactMatch);

  status = atc_transition_compare_to_match(&transition2, &match);
  ACU_ASSERT(status == kAtcCompareWithinMatch);

  status = atc_transition_compare_to_match(&transition3, &match);
  ACU_ASSERT(status == kAtcCompareFarFuture);
}

//---------------------------------------------------------------------------

ACU_TEST(test_atc_transition_storage_add_free_agent_to_active_pool) {
  AtcTransitionStorage ts;
  atc_transition_storage_init(&ts, NULL /*zone_info*/);

  AtcTransition *free_agent = atc_transition_storage_get_free_agent(&ts);
  ACU_ASSERT(free_agent == &ts.transition_pool[0]);
  atc_transition_storage_add_free_agent_to_active_pool(&ts);
  ACU_ASSERT(1 == ts.index_candidate);
  ACU_ASSERT(1 == ts.index_prior);
  ACU_ASSERT(1 == ts.index_free);

  free_agent = atc_transition_storage_get_free_agent(&ts);
  ACU_ASSERT(free_agent == &ts.transition_pool[1]);
  atc_transition_storage_add_free_agent_to_active_pool(&ts);
  ACU_ASSERT(2 == ts.index_candidate);
  ACU_ASSERT(2 == ts.index_prior);
  ACU_ASSERT(2 == ts.index_free);

  free_agent = atc_transition_storage_get_free_agent(&ts);
  ACU_ASSERT(free_agent == &ts.transition_pool[2]);
  atc_transition_storage_add_free_agent_to_active_pool(&ts);
  ACU_ASSERT(3 == ts.index_candidate);
  ACU_ASSERT(3 == ts.index_prior);
  ACU_ASSERT(3 == ts.index_free);

  free_agent = atc_transition_storage_get_free_agent(&ts);
  ACU_ASSERT(free_agent == &ts.transition_pool[3]);
  atc_transition_storage_add_free_agent_to_active_pool(&ts);
  ACU_ASSERT(4 == ts.index_candidate);
  ACU_ASSERT(4 == ts.index_prior);
  ACU_ASSERT(4 == ts.index_free);
}

ACU_TEST(test_atc_transition_storage_add_free_agent_to_candidate_pool) {
  AtcTransitionStorage ts;
  atc_transition_storage_init(&ts, NULL /*zone_info*/);

  // Add the first one to active
  AtcTransition *free_agent = atc_transition_storage_get_free_agent(&ts);
  ACU_ASSERT(free_agent == &ts.transition_pool[0]);
  atc_transition_storage_add_free_agent_to_active_pool(&ts);
  ACU_ASSERT(1 == ts.index_candidate);
  ACU_ASSERT(1 == ts.index_prior);
  ACU_ASSERT(1 == ts.index_free);

  // Add subsequent ones to candidate pool
  free_agent = atc_transition_storage_get_free_agent(&ts);
  ACU_ASSERT(free_agent == &ts.transition_pool[1]);
  atc_transition_storage_add_free_agent_to_candidate_pool(&ts);
  ACU_ASSERT(1 == ts.index_candidate);
  ACU_ASSERT(1 == ts.index_prior);
  ACU_ASSERT(2 == ts.index_free);

  free_agent = atc_transition_storage_get_free_agent(&ts);
  ACU_ASSERT(free_agent == &ts.transition_pool[2]);
  atc_transition_storage_add_free_agent_to_candidate_pool(&ts);
  ACU_ASSERT(1 == ts.index_candidate);
  ACU_ASSERT(1 == ts.index_prior);
  ACU_ASSERT(3 == ts.index_free);

  free_agent = atc_transition_storage_get_free_agent(&ts);
  ACU_ASSERT(free_agent == &ts.transition_pool[3]);
  atc_transition_storage_add_free_agent_to_candidate_pool(&ts);
  ACU_ASSERT(1 == ts.index_candidate);
  ACU_ASSERT(1 == ts.index_prior);
  ACU_ASSERT(4 == ts.index_free);
}

ACU_TEST(test_atc_transition_storage_reserve_prior) {
  AtcTransitionStorage ts;
  atc_transition_storage_init(&ts, NULL /*zone_info*/);

  AtcTransition** prior = atc_transition_storage_reserve_prior(&ts);
  ACU_ASSERT(prior == &ts.transitions[0]);
  ACU_ASSERT(0 == ts.index_prior);
  ACU_ASSERT(1 == ts.index_candidate);
  ACU_ASSERT(1 == ts.index_free);
}

ACU_TEST(test_atc_transition_storage_add_prior_to_candidate_pool) {
  AtcTransitionStorage ts;
  atc_transition_storage_init(&ts, NULL /*zone_info*/);

  AtcTransition** prior = atc_transition_storage_reserve_prior(&ts);
  ACU_ASSERT(prior == &ts.transitions[0]);
  ACU_ASSERT(0 == ts.index_prior);
  ACU_ASSERT(1 == ts.index_candidate);
  ACU_ASSERT(1 == ts.index_free);

  atc_transition_storage_add_prior_to_candidate_pool(&ts);
  ACU_ASSERT(0 == ts.index_prior);
  ACU_ASSERT(0 == ts.index_candidate);
  ACU_ASSERT(1 == ts.index_free);
}

ACU_TEST(test_atc_transition_storage_set_free_agent_as_prior_if_valid) {
  AtcTransitionStorage ts;
  atc_transition_storage_init(&ts, NULL /*zone_info*/);

  // Initial prior
  AtcTransition** prior_reservation =
      atc_transition_storage_reserve_prior(&ts);
  (*prior_reservation)->is_valid_prior = false;
  (*prior_reservation)->transition_time = (AtcDateTuple)
      {2002, 3, 4, 5, kAtcSuffixW};

  // Candiate prior.
  AtcTransition* free_agent = atc_transition_storage_get_free_agent(&ts);
  free_agent->is_valid_prior = true;
  free_agent->transition_time = (AtcDateTuple) {2002, 3, 4, 0, kAtcSuffixW};

  // Should swap because prior->is_valid_prior is false.
  atc_transition_storage_set_free_agent_as_prior_if_valid(&ts);

  // Verify that the two have been swapped.
  AtcTransition* prior = ts.transitions[ts.index_prior];
  free_agent = atc_transition_storage_get_free_agent(&ts);
  ACU_ASSERT(prior->is_valid_prior == true);
  ACU_ASSERT(free_agent->is_valid_prior == false);
  //
  ACU_ASSERT(prior->transition_time.year == 2002);
  ACU_ASSERT(prior->transition_time.month == 3);
  ACU_ASSERT(prior->transition_time.day == 4);
  ACU_ASSERT(prior->transition_time.seconds == 0);
  ACU_ASSERT(prior->transition_time.suffix == kAtcSuffixW);
  //
  ACU_ASSERT(free_agent->transition_time.year == 2002);
  ACU_ASSERT(free_agent->transition_time.month == 3);
  ACU_ASSERT(free_agent->transition_time.day == 4);
  ACU_ASSERT(free_agent->transition_time.seconds == 5);
  ACU_ASSERT(free_agent->transition_time.suffix == kAtcSuffixW);

  // Another Candidate prior.
  free_agent = atc_transition_storage_get_free_agent(&ts);
  free_agent->is_valid_prior = true;
  free_agent->transition_time = (AtcDateTuple) {2002, 3, 4, 6, kAtcSuffixW};

  // Should swap because the transition_time is newer
  atc_transition_storage_set_free_agent_as_prior_if_valid(&ts);

  // Verify that the two have been swapped.
  prior = ts.transitions[ts.index_prior];
  free_agent = atc_transition_storage_get_free_agent(&ts);
  ACU_ASSERT(prior->is_valid_prior == true);
  ACU_ASSERT(free_agent->is_valid_prior == false);
  //
  ACU_ASSERT(prior->transition_time.year == 2002);
  ACU_ASSERT(prior->transition_time.month == 3);
  ACU_ASSERT(prior->transition_time.day == 4);
  ACU_ASSERT(prior->transition_time.seconds == 6);
  ACU_ASSERT(prior->transition_time.suffix == kAtcSuffixW);
  //
  ACU_ASSERT(free_agent->transition_time.year == 2002);
  ACU_ASSERT(free_agent->transition_time.month == 3);
  ACU_ASSERT(free_agent->transition_time.day == 4);
  ACU_ASSERT(free_agent->transition_time.seconds == 0);
  ACU_ASSERT(free_agent->transition_time.suffix == kAtcSuffixW);
}

ACU_TEST(test_atc_transition_storage_add_active_candidates_to_active_pool) {
  AtcTransitionStorage ts;
  atc_transition_storage_init(&ts, NULL /*zone_info*/);

  // create Prior to make it interesting
  AtcTransition** prior = atc_transition_storage_reserve_prior(&ts);
  (*prior)->transition_time = (AtcDateTuple) {1999, 0, 1, 2, kAtcSuffixW};
  (*prior)->match_status = kAtcCompareWithinMatch;

  // Add 3 transitions to Candidate pool, 2 active, 1 inactive.
  AtcTransition* free_agent = atc_transition_storage_get_free_agent(&ts);
  free_agent->transition_time = (AtcDateTuple) {2000, 1, 2, 3, kAtcSuffixW};
  free_agent->match_status = kAtcCompareWithinMatch;
  atc_transition_storage_add_free_agent_to_candidate_pool(&ts);

  free_agent = atc_transition_storage_get_free_agent(&ts);
  free_agent->transition_time = (AtcDateTuple) {2002, 3, 4, 5, kAtcSuffixW};
  free_agent->match_status = kAtcCompareWithinMatch;
  atc_transition_storage_add_free_agent_to_candidate_pool(&ts);

  free_agent = atc_transition_storage_get_free_agent(&ts);
  free_agent->transition_time = (AtcDateTuple) {2001, 2, 3, 4, kAtcSuffixW};
  free_agent->match_status = kAtcCompareFarPast;
  atc_transition_storage_add_free_agent_to_candidate_pool(&ts);

  // Add prior into the Candidate pool.
  atc_transition_storage_add_prior_to_candidate_pool(&ts);

  // Add the actives to the Active pool.
  atc_transition_storage_add_active_candidates_to_active_pool(&ts);

  // Verify that there are 3 transitions in the Active pool.
  ACU_ASSERT(3 == ts.index_prior);
  ACU_ASSERT(3 == ts.index_candidate);
  ACU_ASSERT(3 == ts.index_free);
  ACU_ASSERT(1999 == ts.transitions[0]->transition_time.year);
  ACU_ASSERT(2000 == ts.transitions[1]->transition_time.year);
  ACU_ASSERT(2002 == ts.transitions[2]->transition_time.year);
}

ACU_TEST(test_atc_transition_storage_reset_candidate_pool)
{
  AtcTransitionStorage ts;
  atc_transition_storage_init(&ts, NULL /*zone_info*/);

  // Add 2 transitions to Candidate pool, 2 active, 1 inactive.
  AtcTransition* free_agent = atc_transition_storage_get_free_agent(&ts);
  free_agent->transition_time = (AtcDateTuple) {2000, 1, 2, 3, kAtcSuffixW};
  free_agent->match_status = kAtcCompareWithinMatch;
  atc_transition_storage_add_free_agent_to_candidate_pool(&ts);
  ACU_ASSERT(0 == ts.index_prior);
  ACU_ASSERT(0 == ts.index_candidate);
  ACU_ASSERT(1 == ts.index_free);

  free_agent = atc_transition_storage_get_free_agent(&ts);
  free_agent->transition_time = (AtcDateTuple) {2002, 3, 4, 5, kAtcSuffixW};
  free_agent->match_status = kAtcCompareWithinMatch;
  atc_transition_storage_add_free_agent_to_candidate_pool(&ts);
  ACU_ASSERT(0 == ts.index_prior);
  ACU_ASSERT(0 == ts.index_candidate);
  ACU_ASSERT(2 == ts.index_free);

  // Add active candidates to Active pool. Looks like this
  // already does a resetCandidatePool() effectively.
  atc_transition_storage_add_active_candidates_to_active_pool(&ts);
  ACU_ASSERT(2 == ts.index_prior);
  ACU_ASSERT(2 == ts.index_candidate);
  ACU_ASSERT(2 == ts.index_free);

  // This should be a no-op.
  atc_transition_storage_reset_candidate_pool(&ts);
  ACU_ASSERT(2 == ts.index_prior);
  ACU_ASSERT(2 == ts.index_candidate);
  ACU_ASSERT(2 == ts.index_free);

  // Non-active can be added to the candidate pool.
  free_agent = atc_transition_storage_get_free_agent(&ts);
  free_agent->transition_time = (AtcDateTuple) {2001, 2, 3, 4, kAtcSuffixW};
  free_agent->match_status = kAtcCompareFarPast;
  atc_transition_storage_add_free_agent_to_candidate_pool(&ts);
  ACU_ASSERT(2 == ts.index_prior);
  ACU_ASSERT(2 == ts.index_candidate);
  ACU_ASSERT(3 == ts.index_free);

  // Reset should remove any remaining candidate transitions.
  atc_transition_storage_reset_candidate_pool(&ts);
  ACU_ASSERT(2 == ts.index_prior);
  ACU_ASSERT(2 == ts.index_candidate);
  ACU_ASSERT(2 == ts.index_free);
}

//---------------------------------------------------------------------------

ACU_CONTEXT();

int main()
{
  ACU_RUN_TEST(test_atc_transition_compare_to_match_fuzzy);
  ACU_RUN_TEST(test_atc_transition_compare_to_match);
  ACU_RUN_TEST(test_atc_transition_storage_add_free_agent_to_active_pool);
  ACU_RUN_TEST(test_atc_transition_storage_add_free_agent_to_candidate_pool);
  ACU_RUN_TEST(test_atc_transition_storage_reserve_prior);
  ACU_RUN_TEST(test_atc_transition_storage_add_prior_to_candidate_pool);
  ACU_RUN_TEST(test_atc_transition_storage_set_free_agent_as_prior_if_valid);
  ACU_RUN_TEST(
      test_atc_transition_storage_add_active_candidates_to_active_pool);
  ACU_RUN_TEST(test_atc_transition_storage_reset_candidate_pool);
  ACU_SUMMARY();
}
