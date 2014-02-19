<?php

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
/*
 * theme definitions and preprocessing functions
 * 
 */

/**
 * Implements hook_theme().
 */
function bibdk_voxb_theme() {
    $items['voxb_rating'] = array(
        'variables' => array(
            'object' => NULL,
            'display_only' => FALSE,
            'isbn' => '',
            'pid' => '',
            'title' => '',
        ),
        'template' => 'templates/bibdk_voxb-rating',
    );

    return $items;
}

/**
 * Implements MODULE_preprocess_THEME().
 */
/*function bibdk_voxb_preprocess_voxb_rating(&$variables) {
  $rating_block_class = array();
  $star_class = array();
  $star_class[] = 'star-off';
  // In case the user has the ability to rate, assign respective classes.
  if (!$variables['display_only'] && is_object($profile)) {
    $rating_block_class[] = 'rate-enabled';
    $star_class[] = 'use-ajax';
    } 

  // $variables['rating_block_class'] = implode(' ', $rating_block_class);
  $variables['star_class'] = implode(' ', $star_class);
}*/
?>


