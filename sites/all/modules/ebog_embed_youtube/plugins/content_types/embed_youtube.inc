<?php
/**
 * @file
 *
 */


/**
 * Prepare plugin info.
 */
function ebog_embed_youtube_embed_youtube_ctools_content_types() {
  return array(
    'title' => t('Ebog Embed Youtube'),
    'description' => t('Ebog Embed Youtube description.'),
    'single' => TRUE,
    'category' => array('INLEAD'),
    'no title override' => TRUE,
  );
}

/**
 * Rendering of the block.
 */
function ebog_embed_youtube_embed_youtube_content_type_render($subtype, $conf, $args, $context) {
  $block = new stdClass();
  $block->title = check_plain('Ebog Embed Youtube');
  $block->content = theme('ebog_embed_youtube', $conf);
  return $block;
}


/**
 * 'Edit form' callback for the content type.
 */
function ebog_embed_youtube_embed_youtube_content_type_edit_form(&$form, &$form_state) {
  $conf = $form_state['conf'];
  $form['embed_url'] = array(
    '#type' => 'textfield',
    '#title' => t('url to youtube video'),
    '#size' => 50,
    '#description' => t('example: http://www.youtube.com/watch?v=XwmHoWfwHiw'),
    '#default_value' => !empty($conf['embed_url']) ? $conf['embed_url'] : '',
    '#prefix' => '<div class="clear-block no-float">',
    '#suffix' => '</div>',
  );
  $form['descr'] = array(
    '#type' => 'textfield',
    '#title' => t('description'),
    '#size' => 100,
    '#description' => t('short description of video'),
    '#default_value' => !empty($conf['descr']) ? $conf['descr'] : '',
    '#prefix' => '<div class="clear-block no-float">',
    '#suffix' => '</div>',
  );
  return $form;
}

function ebog_embed_youtube_embed_youtube_content_type_edit_form_submit(&$form, &$form_state) {
  $embed_url = str_replace('http://www.youtube.com/watch?v=', 'http://www.youtube.com/embed/', $form_state['values']['embed_url']);
  $form_state['conf']['embed_url'] = $embed_url;
  $form_state['conf']['descr'] = $form_state['values']['descr'];
}
