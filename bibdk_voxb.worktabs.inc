<?php

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/** Implements hook_worktabs_items
 *
 * @param type $tabs
 * @param type $entity
 * @return type 
 */
function bibdk_voxb_worktabs_items($tabs, $entity) {
  $render_array = bibdk_voxb_get_rating_container($entity);
  
  if ($render_array === FALSE){
    return;
  }
  
  $tabs['reviews']['voxb'] = array(
    'title' => t('VOXB', array(), array('context' => 'voxb')),
    //'btn_txt_closed' => t('read or write review',array(), array('context'=>'voxb')),
    //'btn_txt_open' => '',
    // 'rendered' => drupal_render($trigger_form),
    'rendered' => drupal_render($render_array),
    'weight' => 0,
  );

  return $tabs;
}

function bibdk_voxb_get_rating_container($entity) {
  $isbn = $entity->getISBN();
  
  if (!isset($isbn)) {
    return FALSE;
  }  
  
  $isbn = str_replace('-', '', $isbn);
  $item = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('bibdk_voxb_tab'),
      'data-isbn' => $isbn,
    ),
  );
  $item['#attached']['js'] = array(drupal_get_path('module', 'bibdk_voxb') . '/js/bibdk_voxb_load.js');
  $item['#attached']['css'] = array(drupal_get_path('module', 'bibdk_voxb') . '/css/voxb.css');
  
  return $item;
}

