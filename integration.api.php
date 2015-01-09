<?php
/**
 * @file
 * Demonstraion code for the Integration API.
 */

/**
 * hook_integration().
 *
 * A simple way of integrating feature module while keeping them modular, ie
 * they can be turned off without affecting each other or the site aversely.
 * 
 * An array of data provides information on views, page paths and  bundle types
 * provided by a module so the can be used elsewhere without hardcoding
 * dependencies.
 */
function hook_integration() {
  // Add links to views if a module wants to do that.
  return array(
    'module_name' => array(
      // Entity will be used in add content links and possibly elsewhere.
      'entity' => array(
        'your_entity_type'=> array(
          'your_bundle_type' => array(
            // Provide an add content link for this bundle.
            'add_content_link' => TRUE,
            // Provide a link, path and bundle for content type tabs.
            // The bundle will be available as the value of the 'integration' context,
            // which will be active on the provided subpath.
            'tab' => array(
              'your_label' => array(
                'subpath' => 'subpath',
                'node' => 'bundle',
              ),
            ),
            // Tags field. Enabled by default.
            'tags' => array(
              'enabled' => FALSE,
            ),
            // Image field. Disabled by default.
            'image'=> array(
              'enabled' => TRUE,
            ),
          ),
        ),
      ),
      // This view will be altered in a standard way for integrations, removing
      // group or node subscription flags if needed and removing node flag if not.
      // an add content button is is a group aware add content link to the
      // provided node bundle.
      'views' => array(
        'view' => array(
          'display_id' => array(
            'add_content_link' => array(
              'areas' => array('header','footer'),
            ),
            'subscribe_flag' => array(
              'group_type' => 'node',
              'entity_type' => 'node',
            ),
            // Tag filtering. Enabled by default.
            'tag_filter'=> array(
              'enabled' => FALSE,
            ),
          ),
        ),
      ),
    ),
  );
}

