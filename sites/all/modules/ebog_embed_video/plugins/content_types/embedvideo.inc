<?php
/**
 * @file
 *
 */


/**
 * Prepare plugin info.
 */
function ebog_embed_video_embedvideo_ctools_content_types() {
  return array(
    'title' => t('Ebog Embed video'),
    'description' => t('Ebog Embed video description.'),
    'single' => TRUE,
    'category' => array('INLEAD'),
    //'no title override' => TRUE,
  );
}

/**
 * Rendering of the block.
 */
function ebog_embed_video_embedvideo_content_type_render($subtype, $conf, $args, $context) {
  $block = new stdClass();
  $block->title = check_plain('Ebog Embed video');
  $block->content = theme('ebog_embed_video', $conf);
  return $block;
}

/**
 * 'Edit form' callback for the content type.
 */
function ebog_embed_video_embedvideo_content_type_edit_form(&$form, &$form_state) {
  $conf = $form_state['conf'];
  $form['embed_youtube_url'] = array(
    '#type' => 'textfield',
    '#title' => t('url to youtube video'),
    '#size' => 50,
    '#description' => t('example: http://www.youtube.com/watch?v=XwmHoWfwHiw'),
    '#default_value' => !empty($conf['embed_youtube_url']) ? $conf['embed_youtube_url'] : '',
    '#prefix' => '<div class="clear-block no-float">',
    '#suffix' => '</div>',
  );
  $form['embed_vimeo_url'] = array(
    '#type' => 'textfield',
    '#title' => t('url to vimeo video'),
    '#size' => 50,
    '#description' => t('example: http://vimeo.com/3535418'),
    '#default_value' => !empty($conf['embed_vimeo_url']) ? $conf['embed_vimeo_url'] : '',
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

function ebog_embed_video_embedvideo_content_type_edit_form_submit(&$form, &$form_state) {

  $youtube_url = $form_state['values']['embed_youtube_url'];
  $query = parse_url($youtube_url, PHP_URL_QUERY);
  $query = explode('&',$query);
  $embed_youtube_code = '';
  foreach ($query as $param) {
    if (strrpos($param, 'v=') == 0) {
      $embed_youtube_code = substr($param, 2);
      break;
    }
  }
  $form_state['conf']['embed_youtube_url'] = $form_state['values']['embed_youtube_url'];
  $form_state['conf']['embed_youtube_code'] = $embed_youtube_code;

  $vimeo_url = $form_state['values']['embed_vimeo_url'];
  $embed_vimeo_code = str_replace('http://vimeo.com/', '', $vimeo_url);
  $form_state['conf']['embed_vimeo_url'] = $form_state['values']['embed_vimeo_url'];
  $form_state['conf']['embed_vimeo_code'] = $embed_vimeo_code;

  $form_state['conf']['descr'] = $form_state['values']['descr'];

}
