<?php

/**
 * @file
 * Integration with pathauto alias generation
 */

/**
 * Batch processing callback; Generate aliases for library news pages.
 */
function ding_path_alias_library_news_page_pathauto_bulk_update_batch_process(&$context) {
  if (!isset($context['sandbox']['current'])) {
    $context['sandbox']['count'] = 0;
    $context['sandbox']['current'] = 0;
  }

  $query = db_select('node', 'n');
  $query->leftJoin('url_alias', 'ua', "CONCAT(CONCAT('node/', n.nid), '/news') = ua.source");
  $query->addField('n', 'nid');
  $query->isNull('ua.source');
  $query->condition('n.nid', $context['sandbox']['current'], '>');
  $query->condition('n.type', 'ding_library', '=');
  $query->orderBy('n.nid');
  $query->addTag('pathauto_bulk_update');

  // Get the total amount of items to process.
  if (!isset($context['sandbox']['total'])) {
    $context['sandbox']['total'] = $query->countQuery()->execute()->fetchField();

    // If there are no nodes to update, the stop immediately.
    if (!$context['sandbox']['total']) {
      $context['finished'] = 1;
      return;
    }
  }

  $query->range(0, 25);
  $nids = $query->execute()->fetchCol();
  ding_path_alias_library_news_page_pathauto_bulk_update_alias_multiple($nids, 'bulkupdate');

  $context['sandbox']['count'] += count($nids);
  $context['sandbox']['current'] = max($nids);
  $context['message'] = t('Updated alias for news page for library node with nid @nid.', array('@nid' => end($nids)));

  if ($context['sandbox']['count'] != $context['sandbox']['total']) {
    $context['finished'] = $context['sandbox']['count'] / $context['sandbox']['total'];
  }
  elseif (!empty($gids)) {
    $context['finished'] = 1;
  }
}

/**
 * Update the URL aliases for multiple library news pages.
 *
 * @param $nids
 *   An array of node IDs.
 * @param $op
 *   Operation being performed on the nodes ('insert', 'update' or
 *   'bulkupdate').
 * @param $options
 *   An optional array of additional options.
 */
function ding_path_alias_library_news_page_pathauto_bulk_update_alias_multiple($nids, $op, array $options = array()) {
  $options += array('message' => FALSE);

  $nodes = node_load_multiple($nids);
  foreach ($nodes as $node) {
    ding_path_alias_library_news_page_pathauto_update_alias($node, $op, $options);
  }

  if (!empty($options['message'])) {
    drupal_set_message(format_plural(count($nids), 'Updated URL alias for 1 library news page.', 'Updated URL aliases for @count library news pages.'));
  }
}

/**
 * Update the URL aliases for an individual node.
 *
 * @param $node
 *   A fully loaded node.
 * @param $op
 *   Operation being performed on the node ('insert', 'update' or 'bulkupdate').
 * @param $options
 *   An optional array of additional options.
 */
function ding_path_alias_library_news_page_pathauto_update_alias(stdClass $node, $op, array $options = array()) {
  module_load_include('inc', 'pathauto');
  $path = 'node/' . $node->nid . '/news';

  // Add language info.
  $options += array(
    'language' => isset($node->language) ? $node->language : LANGUAGE_NONE,
  );

  pathauto_create_alias('ding_path_alias_library_news_page', $op, $path, array('node' => $node), 'ding_library', $options['language']);
}

/**
 * Batch processing callback; Generate aliases for library event pages.
 */
function ding_path_alias_library_event_page_pathauto_bulk_update_batch_process(&$context) {
  if (!isset($context['sandbox']['current'])) {
    $context['sandbox']['count'] = 0;
    $context['sandbox']['current'] = 0;
  }

  $query = db_select('node', 'n');
  $query->leftJoin('url_alias', 'ua', "CONCAT(CONCAT('library/', n.nid), '/events') = ua.source");
  $query->addField('n', 'nid');
  $query->isNull('ua.source');
  $query->condition('n.nid', $context['sandbox']['current'], '>');
  $query->condition('n.type', 'ding_library', '=');
  $query->addTag('pathauto_bulk_update');

  // Get the total amount of items to process.
  if (!isset($context['sandbox']['total'])) {
    $context['sandbox']['total'] = $query->countQuery()->execute()->fetchField();

    // If there are no nodes to update, the stop immediately.
    if (!$context['sandbox']['total']) {
      $context['finished'] = 1;
      return;
    }
  }

  $query->range(0, 25);
  $nids = $query->execute()->fetchCol();
  ding_path_alias_library_event_page_pathauto_bulk_update_alias_multiple($nids, 'bulkupdate');

  $context['sandbox']['count'] += count($nids);
  $context['sandbox']['current'] = max($nids);
  $context['message'] = t('Updated alias for event page for library node with nid @nid.', array('@nid' => end($nids)));

  if ($context['sandbox']['count'] != $context['sandbox']['total']) {
    $context['finished'] = $context['sandbox']['count'] / $context['sandbox']['total'];
  }
  elseif (!empty($nids)) {
    $context['finished'] = 1;
  }
}

/**
 * Update the URL aliases for multiple library event pages.
 *
 * @param $nids
 *   An array of node IDs.
 * @param $op
 *   Operation being performed on the nodes ('insert', 'update' or
 *   'bulkupdate').
 * @param $options
 *   An optional array of additional options.
 */
function ding_path_alias_library_event_page_pathauto_bulk_update_alias_multiple($nids, $op, array $options = array()) {
  $options += array('message' => FALSE);

  $nodes = node_load_multiple($nids);
  foreach ($nodes as $node) {
    ding_path_alias_library_event_page_pathauto_update_alias($node, $op, $options);
  }

  if (!empty($options['message'])) {
    drupal_set_message(format_plural(count($nids), 'Updated URL alias for 1 library event page.', 'Updated URL aliases for @count library event pages.'));
  }
}

/**
 * Update the URL aliases for an individual node.
 *
 * @param $node
 *   A fully loaded node.
 * @param $op
 *   Operation being performed on the node ('insert', 'update' or 'bulkupdate').
 * @param $options
 *   An optional array of additional options.
 */
function ding_path_alias_library_event_page_pathauto_update_alias(stdClass $node, $op, array $options = array()) {
  module_load_include('inc', 'pathauto');
  $path = 'node/' . $node->nid . '/events';

  // Add language info.
  $options += array(
    'language' => isset($node->language) ? $node->language : LANGUAGE_NONE,
  );

  pathauto_create_alias('ding_path_alias_library_event_page', $op, $path, array('node' => $node), 'ding_library', $options['language']);
}

/**
 * Batch processing callback; Generate aliases for library about pages.
 */
function ding_path_alias_library_about_page_pathauto_bulk_update_batch_process(&$context) {
  if (!isset($context['sandbox']['current'])) {
    $context['sandbox']['count'] = 0;
    $context['sandbox']['current'] = 0;
  }

  $query = db_select('node', 'n');
  $query->leftJoin('url_alias', 'ua', "CONCAT(CONCAT('node/', n.nid), '/about') = ua.source");
  $query->addField('n', 'nid');
  $query->isNull('ua.source');
  $query->condition('n.nid', $context['sandbox']['current'], '>');
  $query->condition('n.type', 'ding_library', '=');
  $query->orderBy('n.nid');
  $query->addTag('pathauto_bulk_update');

  // Get the total amount of items to process.
  if (!isset($context['sandbox']['total'])) {
    $context['sandbox']['total'] = $query->countQuery()->execute()->fetchField();

    // If there are no nodes to update, the stop immediately.
    if (!$context['sandbox']['total']) {
      $context['finished'] = 1;
      return;
    }
  }

  $query->range(0, 25);
  $nids = $query->execute()->fetchCol();
  ding_path_alias_library_about_page_pathauto_bulk_update_alias_multiple($nids, 'bulkupdate');

  $context['sandbox']['count'] += count($nids);
  $context['sandbox']['current'] = max($nids);
  $context['message'] = t('Updated alias for about page for library node with nid @nid.', array('@nid' => end($nids)));

  if ($context['sandbox']['count'] != $context['sandbox']['total']) {
    $context['finished'] = $context['sandbox']['count'] / $context['sandbox']['total'];
  }
  elseif (!empty($nids)) {
    $context['finished'] = 1;
  }
}

/**
 * Update the URL aliases for multiple library about pages.
 *
 * @param $nids
 *   An array of node IDs.
 * @param $op
 *   Operation being performed on the nodes ('insert', 'update' or
 *   'bulkupdate').
 * @param $options
 *   An optional array of additional options.
 */
function ding_path_alias_library_about_page_pathauto_bulk_update_alias_multiple($nids, $op, array $options = array()) {
  $options += array('message' => FALSE);

  $nodes = node_load_multiple($nids);
  foreach ($nodes as $node) {
    ding_path_alias_library_about_page_pathauto_update_alias($node, $op, $options);
  }

  if (!empty($options['message'])) {
    drupal_set_message(format_plural(count($nids), 'Updated URL alias for 1 library about page.', 'Updated URL aliases for @count library about pages.'));
  }
}

/**
 * Update the URL aliases for an individual node.
 *
 * @param $node
 *   A fully loaded node.
 * @param $op
 *   Operation being performed on the node ('insert', 'update' or 'bulkupdate').
 * @param $options
 *   An optional array of additional options.
 */
function ding_path_alias_library_about_page_pathauto_update_alias(stdClass $node, $op, array $options = array()) {
  module_load_include('inc', 'pathauto');
  $path = 'node/' . $node->nid . '/about';

  // Add language info.
  $options += array(
    'language' => isset($node->language) ? $node->language : LANGUAGE_NONE,
  );

  pathauto_create_alias('ding_path_alias_library_about_page', $op, $path, array('node' => $node), 'ding_library', $options['language']);
}
