<?php

/**
 * Build the main tab bar for groups and users.
 *
 * @return array - a drupal render array
 */
function collabco_integration_tabs_integration($path, $subpaths = array()) {
  $links = collabco_integration_tabs_integration_links($path, $subpaths);
  $tabs = array(
    'header-content-tab' => array(
      '#theme' => 'item_list',
      '#weight' => 20,
      '#items' => $links,
      '#prefix' => '<div class="header-content-tab">',
      '#suffix' => '</div>',
    ),
  );
  return $tabs;
}

/**
 * Build the links for group or user tabs from whatever features 
 * provide an integration hook with the required details.
 *
 * @return array - a drupal render array
 */
function collabco_integration_tabs_integration_links($path, $subpaths = array()) {
  $links = array();
  $current_path = menu_get_item();
  $integrations = module_invoke_all('collabco_integration');
  // Add link path and label from integration, $subpaths may allready be
  // populated.
  foreach($integrations as $integration) {
    if (empty($integration['entity']['node'])) {
      continue;
    }
    foreach($integration['entity']['node'] as $bundle => $options) {
      if (!empty($options['tab'])) {
        foreach($options['tab'] as $label => $tab_options) {
          $subpaths[$label] = '/' . $tab_options['subpath'];
        }
      }
    }
  }
  // Process all links.
  foreach($subpaths as $label => $subpath) {
    $link_path = $path . $subpath;
    $links[$subpath]['data'] = l($label, $link_path);
    // Set active li for styling.
    if ($link_path == $current_path['href']) {
      $links[$subpath]['class'] = array('active');
    }
  }
  return $links;
}

/**
 * Add a permissions aware create-content link to a view area.
 */
function collabco_integration_views_content_link($view, $display_id, $bundles, $areas) {
  global $user;
  $group = og_context();
  $gid = !empty($group['gid']) ? $gid = $group['gid'] : NULL; 
  $links = array(
    '#theme' => 'item_list',
    '#attributes' => array('class' => array('add-content-links')),
    '#items' => collabco_integration_build_node_links($bundles, $gid)
  );
  // Views wont render the array for us so we render now.
  $rendered_links = drupal_render($links);
  if ($links) {
    $options = array(
       'id' => 'area',
       'table' => 'views',
       'field' => 'area',
       'empty' => TRUE,
       'content' => $rendered_links,
       'format' => 'filtered_html',
       'tokenize' => 0,
    );
    foreach ($areas as $area) {
      $view->add_item($display_id, $area, 'views', 'area', $options);
    }
  }
}

function collabco_integration_build_node_links($bundles, $gid = NULL, $options = array()) {
  // Add menu link if the current user can create nodes in the bundle.
  $items = array();
  $group_type = 'node';
  // Build query array for entity reference prepoluate.
  $options = (!empty($gid) ? array_merge($options, collabco_groups_feature_prepopulate_query($gid)) : array());

  // This is a group page. Add links only if the user has group permissions.
  if (!empty($gid)) {
    if (!og_is_member($group_type, $gid)) {
      return;
    }
    $group = entity_load($group_type, array($gid));
    foreach ($bundles as $bundle) {
      // Construct permission name string for the node bundle.
      $string = "create $bundle content";
      if (og_user_access($group_type, $gid, $string)) {
        $text = t('Create ') . node_type_get_name($bundle);
        $url = "node/add/" . str_replace('_', '-', $bundle);
        $items[$bundle] = l($text, $url, $options); 
      }
    }
    return $items;
  }
  // Add links if the user has standard permissions.
  foreach ($bundles as $bundle) {
    // Construct permission name string for the node bundle.
    $string = "create $bundle content";
    if (user_access($string)) {
      $text = t('Create ') . node_type_get_name($bundle);
      $url = "node/add/" . str_replace('_', '-', $bundle);
      $items[$bundle] = l($text, $url, $options); 
    }
  }
  return $items;
}  

