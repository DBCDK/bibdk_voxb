<?php
/**
 * @file
 * bibdk_voxb_blog_fields.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function bibdk_voxb_blog_fields_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}
