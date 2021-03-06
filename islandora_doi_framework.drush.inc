<?php

/**
 * @file
 * Drush file for the Islandora DOI module.
 */

/**
 * Implements hook_drush_command().
 */
function islandora_doi_framework_drush_command() {
  $items = array();
  $items['islandora_doi_framework_assign_dois'] = array(
    'aliases' => array('idfad'),
    'description' => 'Assign DOIs to a list of objects.',
    'examples' => array(
      'drush islandora_doi_frameork_assign_dois --user=admin --pid_file=/tmp/pids.txt',
    ),
    'options' => array(
      'pid_file' => array(
        'description' => 'Absolute path to a file containing PIDs of objects to assign DOIs to, one per line.',
      ),
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );
  $items['islandora_doi_framework_update_dois'] = array(
    'aliases' => array('idfud'),
    'description' => 'Update the DOIs of a list of objects.',
    'examples' => array(
      'drush islandora_doi_frameork_update_dois --user=admin --pid_file=/tmp/dois.txt',
    ),
    'options' => array(
      'pid_file' => array(
        'description' => 'Absolute path to a file containing PIDs of objects whose DOIs are ' .
        'to be updated, plus the corresponding DOIs and an action, which is one of "url", ' .
        '"metadata", or "both" (the default). One tab-separated entry per line.',
      ),
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );
  return $items;
}

/**
 * Assigns DOIs to objects.
 */
function drush_islandora_doi_framework_assign_dois() {
  module_load_include('inc', 'islandora_doi_framework', 'includes/utilities');

  // Check to see if there is an input file and if there is, use the PIDs
  // from it.
  if (drush_get_option('pid_file')) {
    $input_file_path = drush_get_option('pid_file');
    $pids = islandora_doi_framework_read_input_file($input_file_path);
  }

  // Cycle through the objects and assign the DOIs.
  foreach ($pids as $pid) {
    if ($doi = islandora_doi_framework_mint_doi($pid)) {
      if (islandora_doi_framework_persist_doi($doi, $pid)) {
        drush_log(dt('DOI !doi assigned to object !pid',
          array('!doi' => $doi, '!pid' => $pid)), 'ok');
      }
    }
  }
}

/**
 * Updates DOIs.
 *
 * Note: this functionality is not yet complete. DOIs need to be present
 * in the PID file.
 */
function drush_islandora_doi_framework_update_dois() {
  module_load_include('inc', 'islandora_doi_framework', 'includes/utilities');

  // Check to see if there is an input file and if there is, use the data
  // from it.
  if (drush_get_option('pid_file')) {
    $input_file_path = drush_get_option('pid_file');
    $rows = islandora_doi_framework_read_input_file($input_file_path);
  }

  // Cycle through the objects and update the DOIs.
  foreach ($rows as $row) {
    list($pid, $action) = explode("\t", $row);
    if (islandora_doi_framework_update_doi($pid, $action)) {
      drush_log(dt('DOI for object !pid (!doi) updated.',
        array('!doi' => $doi, '!pid' => $pid)), 'ok');
    }
  }
}
