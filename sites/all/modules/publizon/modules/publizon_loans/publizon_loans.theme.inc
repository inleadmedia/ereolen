<?php
/**
 * @file
 *
 */


/**
 * Default implementation of hook_publizon_product_preprocess().
 *
 * @param type $vars
 */
function template_preprocess_publizon_loan(&$vars) {
  $ting_object = $vars['loan']->product->getTingObject();

  // Prepare variables.
  $vars['object_link'] = 'ting/object/' . (string)$ting_object->id;
  $vars['title']  = l($ting_object->record['dc:title'][''][0], $vars['object_link'], array(
    'attributes' => array(
      'class' => 'title',
    )
  ));
  $vars['authors'] = publizon_get_authors($ting_object);



  // Implement hook_buttons.

}
