<?php

/**
 * Drupal public (system://) stream wrapper class.
 *
 * Provides support for storing publicly accessible files with the Drupal file
 * interface.
 */
abstract class SystemStreamWrapper extends DrupalLocalStreamWrapper {

  protected function getSystemName($uri = NULL) {
    if (!isset($uri)) {
      $uri = $this->uri;
    }
    list($scheme, $target) = explode('://', $uri, 2);
    $pos = strpos($target, '/');
    return $pos === FALSE ? $target : substr($target, 0, $pos);
  }

  protected function getTarget($uri = NULL) {
    if (!isset($uri)) {
      $uri = $this->uri;
    }

    list($scheme, $target) = explode('://', $uri, 2);

    // Remove erroneous leading or trailing, forward-slashes and backslashes.
    $target = trim($target, '\/');
    $target = substr($target, strlen($this->getSystemName()));
    $target = trim($target, '\/');
    return $target;
  }

  /**
   * Gets the path that the wrapper is responsible for.
   *
   * @return
   *   String specifying the path.
   */
  //abstract public function getDirectoryPath();

  /**
   * Overrides getExternalUrl().
   *
   * Return the HTML URI of a public file.
   */
  public function getExternalUrl() {
    $path = str_replace('\\', '/', $this->getTarget());
    return $GLOBALS['base_url'] . '/' . $this->getDirectoryPath() . '/' . drupal_encode_path($path);
  }

  /**
   * DrupalStreamWrapperInterface requires that these methods be implemented,
   * but none of them apply to a read-only stream wrapper. On failure they
   * are expected to return FALSE.
   */

  public function stream_write($data) {
    return FALSE;
  }
  public function unlink($uri) {
    // Although the remote file itself can't be deleted, return TRUE so that
    // file_delete() can remove the file record from the database.
    return TRUE;
  }
  public function rename($from_uri, $to_uri) {
    return FALSE;
  }
  public function mkdir($uri, $mode, $options) {
    return FALSE;
  }
  public function rmdir($uri, $options) {
    return FALSE;
  }
  public function chmod($mode) {
    return FALSE;
  }
  public function dirname($uri = NULL) {
    return FALSE;
  }
}

class ModuleSystemStreamWrapper extends SystemStreamWrapper {

  /**
   * Implements abstract public function getDirectoryPath()
   */
  public function getDirectoryPath() {
    return drupal_get_path('module', $this->getSystemName());
  }
}

class ThemeSystemStreamWrapper extends SystemStreamWrapper {

  /**
   * Implements abstract public function getDirectoryPath()
   */
  public function getDirectoryPath() {
    return drupal_get_path('theme', $this->getSystemName());
  }
}
