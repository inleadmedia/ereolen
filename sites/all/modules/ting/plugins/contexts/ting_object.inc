<?php

/**
 * @file
 *
 * Plugin to provide a node context. A node context is a node wrapped in a
 * context object that can be utilized by anything that accepts contexts.
 */

/**
 * Implementation of specially named hook_ctools_contexts().
 */
function ting_ting_object_ctools_contexts() {
  $args = array();
  $args['ting_object'] = array(
    'title' => t('Ting object'),
    'description' => t('An object from the Ting database – books, CD-ROMs, magazines, etc.'),
    'context' => 'ting_ting_object_create',
    'settings form' => 'ting_ting_object_context_settings_form',
    'keyword' => 'ting_object',
    'context name' => 'ting_object',
    'convert list' => 'ting_ting_object_convert_list',
    'convert' => 'ting_ting_object_convert',
    'placeholder form' => array(
      '#type' => 'textfield',
      '#description' => t('Enter the object ID for this context'),
    ),
  );
  return $args;
}

/**
 * It's important to remember that $conf is optional here, because contexts
 * are not always created from the UI.
 */
function ting_ting_object_create($empty, $data = NULL, $conf = FALSE) {
  $context = new ctools_context('ting_object');
  $context->plugin = 'ting_object';

  if ($empty) {
    return $context;
  }

  $object_id = is_array($data) && isset($data['object_id']) ? $data['object_id'] : (is_object($data) ? $data->id : 0);
  if ($conf) {
    module_load_include('client.inc', 'ting');
    $data = ting_get_object_by_id($object_id, TRUE);
  }

  if (!empty($data)) {
    $context->data     = $data;
    $context->argument = $object_id;
    if (!empty($data->creators_string)) {
      $context->title  = t('@title by @author', array('@title' => $data->title, '@author' => $data->creators_string));
    }
    else {
      $context->title  = check_plain($data->title);
    }

    return $context;
  }
}

/**
 * Settings for when manually creating this context.
 */
function ting_ting_object_context_settings_form($conf) {
  $form = array();
  return $form;
}

/**
 * List of conversion options.
 */
function ting_ting_object_convert_list() {
  return array(
    'title_full' => t('Object title and author names'),
    //'title' => t('Object title'),
  );
}

/**
 * Convert a context into a string.
 */
function ting_ting_object_convert($context, $type) {
  switch ($type) {
    case 'title_full':
      return $context->title;
  }
}

