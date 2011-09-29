<?php
/**
 * @file
 *
 */


/**
 * Prepare plugin info.
 */
function litteratursiden_feed_compare_feed_and_compare_page_ctools_content_types() {
  return array(
    'title' => t('Litteratursiden feed and compare (page)'),
    'description' => t('Litteratursiden feed and compare (page), description.'),
    'single' => TRUE,
    'category' => array('INLEAD'),
  );
}

/**
 * Rendering of the block.
 */
function litteratursiden_feed_compare_feed_and_compare_page_content_type_render($subtype, $conf, $args, $context) {
  $block = new stdClass();
  $block->title = check_plain('Feed and Compare (page)');

  $items_on_page = variable_get('litteratursiden_feed_compare_items_on_page', '10');
  $items = litteratursiden_feed_compare_get_cached_feed($items_on_page);

  $block->content = theme('litteratursiden_feed_compare_page', $items);
  return $block;
}

/**
 * 'Edit form' callback for the content type.
 */
function litteratursiden_feed_compare_feed_and_compare_page_content_type_edit_form(&$form, &$form_state) {
  return $form;
}
