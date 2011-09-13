<?php
/**
 * @file
 *
 */

$plugin = array(
  'title' => t('Litteratursiden feed and compare (front)'),
  'description' => t('Litteratursiden feed and compare, (front) description.'),
  'single' => TRUE,
  'content_types' => array('feed_and_compare_front'),
  'render callback' => 'litteratursiden_feed_compare_feed_and_compare_front_render',
  'defaults' => array(),
  'category' => array('INLEAD'),
);

/**
 * Rendering of the block.
 */
function litteratursiden_feed_compare_feed_and_compare_front_render($subtype, $conf, $args, $context) {
  $block = new stdClass();
  $block->title = check_plain('Feed and Compare (front)');

  $items_on_front = variable_get('litteratursiden_feed_compare_items_on_front', '2');
  $items = litteratursiden_feed_compare_get_cached_feed($items_on_front);

  $block->content = theme('litteratursiden_feed_compare_front', $items);
  return $block;
}
