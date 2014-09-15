<?php
function bibdk_voxb_offensive_form($voxb_id) {
  $form = array(
    'but' => array(
      '#name' => $voxb_id,
      '#type' => 'submit',
      '#value' => t('bibdk_offensive_content', array(), array('context' => 'bibdk_voxb')),
    ),
  );

  return $form;
}